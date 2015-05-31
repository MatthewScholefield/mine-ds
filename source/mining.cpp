#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include "world.h"
#include "chests.h"
#include "furnaceHandler.h"
#include "graphics/graphics.h"
#include "graphics/inventoryGraphics.h"
#include "inventory.h"
#include "blockName.h"
#include "blocks.h"
#include "mainGame.h"
#include "mining.h"
#include "Config.h"
#include "general.h"
#include "mobs/mobHandler.h"
#include "mobs/hurt.h"

bool miningDisabled = false;
int framesOnBlock;
int framesTillBreak;
int targetSum = -1;
bool layerIsBG; //Layer of mined block
bool finishedTask = false;

void setMiningDisabled(bool set) //false enables block destroy, true disable it
{
	miningDisabled = set;
}

bool canMine() //Returns whether touch input can destroy blocks
{
	return !miningDisabled;
}

/*void mineBlock(WorldObject* world, int x, int y, bool bg) //True if block broken
{
	int selectedBlock = getBlockID(invSlot);
	if (getType(selectedBlock) == SWORD)
	{
		skipLightUpdate = true;
		return; //Swords don't mine blocks
	}
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	bool stylusMoved = (miningX == x || miningY == y) ? false : true;
	if (isSurvival() || blockXY == CHEST)
	{
		if (stylusMoved)
		{
			miningX = x;
			miningY = y;
			int handHardness = (getHardness(selectedBlock) < 0) ? getHardness(selectedBlock)*-1 : 1; // Hardness of item in hand
			int blockTypeXY = getType(blockXY);
			switch (getType(selectedBlock)) // Check if we are using the correct tool
			{
				case AXE:
					if (blockTypeXY != WOOD) handHardness = 1;
					break;
				case PICKAXE:
					if (blockTypeXY != STONEBLOCK) handHardness = 1;
					break;
				case SHOVEL:
					if (blockTypeXY != SOIL) handHardness = 1;
					break;
			}
			mining = getHardness(blockXY)*10 / handHardness;
			miningRate = 1;
			framesOnBlock = 0;
		}
		else
			mining -= miningRate;
		++framesOnBlock;
		if (mining > 0)
		{
			skipLightUpdate = true;
			return;
		}
		miningX = -1;
		miningY = -1;
	}
	if (canBreak(blockXY))
	{

		if (canDropItem(blockXY))
			createItemMob(x, y, genericBlock(blockXY));
		else if (!isSurvival())
			addInventory(blockXY);
		if (blockXY == CHEST)
			destroyChest(world, x, y, bg);
		blockXY = AIR;
		hasChangedBlock = true;
	}
}

void setBlock(WorldObject* world, int x, int y)
{
	int selectedBlock = getBlockID(invSlot);
	skipLightUpdate = false;
	bool isCrouched = (keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH));
	bool isPlaceableBlock = (selectedBlock != AIR && !item(selectedBlock) && (!hasChangedBlock || getGlobalSettings()->getProperty(PROPERTY_DRAW)));
	if (isCrouched) // Background
	{
		skipLightUpdate = true; //Background does not affect light
		if (world->blocks[x][y] != AIR && !isBlockWalkThrough(world->blocks[x][y])) // Foreground not empty don't place or mine
			return;
		else if (world->bgblocks[x][y] != AIR) // Mine block
		{
			if (hasChangedBlock == false)
				mineBlock(world, x, y, isCrouched);
		}
		else // Place block
		{
			if (isPlaceableBlock)
			{
				if (subInventory(selectedBlock, 1))
				{
					if (selectedBlock == CHEST)
						createChest(world, x, y, true);
					else
						world->bgblocks[x][y] = selectedBlock;
					if (selectedBlock == SNOW_GRASS && world->bgblocks[x][y - 1] == AIR)
						world->bgblocks[x][y - 1] = SNOW_TOP;
				}
				hasChangedBlock = true;
			}
		}
	}
	else // Foreground
	{
		if (world->blocks[x][y] != AIR) // Mine block
		{
			if (hasChangedBlock == false)
				mineBlock(world, x, y, isCrouched);
			else
				skipLightUpdate = true;
		}
		else // Place block
		{
			if (isPlaceableBlock)
			{
				if (subInventory(selectedBlock, 1))
				{
					if (selectedBlock == CHEST)
						createChest(world, x, y, false);
					else
						world->blocks[x][y] = selectedBlock;
					if (selectedBlock == SNOW_GRASS && world->blocks[x][y - 1] == AIR)
						world->blocks[x][y - 1] = SNOW_TOP;
				}
				hasChangedBlock = true;
			}
			else
			{
				skipLightUpdate = true;
				return;
			}
		}
	}
	//Send a WIFI Update now, if wifi is enabled!
	if (isWifi() && hasChangedBlock == true)
	{
		lastX = x;
		lastY = y;
		placeBlock(x, y);
	}
}

void miningUpdate(WorldObject* world, int a, int b, touchPosition touch, int keys) // keys = keysDown();
{
	if (disableTouchMine)
		return;
	if (keys & KEY_TOUCH)
	{
		int x = (touch.px + a) / 16;
		int y = (touch.py + b) / 16;
		int mobNum = isMobAt(touch.px + a, touch.py + b);
		if (mobNum != -1)
		{
			int damage;
			switch (getBlockID(getSelectedSlot()))
			{
				case SWORD_DIAMOND:
					damage = 6;
					break;
				case SWORD_IRON:
					damage = 4;
					break;
				case SWORD_GOLD:
					damage = 3;
					break;
				case SWORD_STONE:
					damage = 3;
					break;
				case SWORD_WOOD:
					damage = 2;
					break;
				default:
					damage = 1;
					break;
			}
			mobHandlerHurtMob(mobNum, damage, PLAYER_HURT);
		}
		else if (canPlaceBlocks)
		{
			setBlock(world, x, y);
			if (!skipLightUpdate)
				updateBrightnessAround(world, x, y);
		}
	}
	else if (keysHeld() & KEY_TOUCH)
	{
		int x = (touch.px + a) / 16;
		int y = (touch.py + b) / 16;
		setBlock(world, x, y);
		if (!skipLightUpdate)
			updateBrightnessAround(world, x, y);
	}
	else if (framesOnBlock > 0 && framesOnBlock < 30 && world->blocks[miningX][miningY] == CHEST)
	{
		openChest(world, miningX, miningY, false);
		framesOnBlock = 0;
	}
	else if (framesOnBlock > 0 && framesOnBlock < 30 && world->bgblocks[miningX][miningY] == CHEST)
	{
		openChest(world, miningX, miningY, true);
		framesOnBlock = 0;
	}
	else
	{
		hasChangedBlock = false;
		framesOnBlock = 0;
	}
	if (keys & getGlobalSettings()->getKey(ACTION_ITEM_LEFT))
	{
		if (!isSurvival() && keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH))
			changeBlockPage(false);
		else
		{
			int origSlot = invSlot;
			--invSlot;
			if (invSlot < 0) invSlot = NUM_INV_SPACES - 1;
			while ((checkInventorySlot(invSlot) == 0 || getBlockID(invSlot) == AIR) && invSlot != origSlot)
			{
				--invSlot;
				if (invSlot < 0) invSlot = NUM_INV_SPACES - 1;
			}
		}
		calculateTopBlock();
	}
	else if (keys & getGlobalSettings()->getKey(ACTION_ITEM_RIGHT))
	{
		if (!isSurvival() && keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH))
			changeBlockPage(true);
		else
		{
			int origSlot = invSlot;
			++invSlot;
			if (invSlot < 0) invSlot = NUM_INV_SPACES - 1;
			while ((checkInventorySlot(invSlot) == 0 || getBlockID(invSlot) == AIR) && invSlot != origSlot)
			{
				++invSlot;
				if (invSlot >= NUM_INV_SPACES) invSlot = 0;
			}
		}
		calculateTopBlock();
	}
	if (invSlot < 0)
	{
		invSlot = NUM_INV_SPACES - 1;
		calculateTopBlock();
	}
	else if (invSlot >= NUM_INV_SPACES)
	{
		invSlot = 0;
		calculateTopBlock();
	}
	if (getBlockID(invSlot) != oldBlockID) calculateTopBlock();
	oldBlockID = getBlockID(invSlot);
	//Draw the selected block
	if (getBlockID(invSlot) != 0)
		showGraphic(&topBlock, 0, 48);
	if (canPlaceBlocks == false) ++frameCounting;
	if (frameCounting > 60)
	{
		//More than a second, and no block confirm?
		placeBlock(lastX, lastY);
		frameCounting = 0;
		++failedAttempts;
	}
}*/

void destroyBlock(WorldObject *world, int x, int y, bool bg)
{
	int *blockXY = bg ? &world->bgblocks[x][y] : &world->blocks[x][y];
	switch (*blockXY)
	{
		case CHEST:
			destroyChest(world, x, y, bg);
			break;
		case FURNACE:
			destroyFurnace(world, x, y, bg);
			break;
		default:
			if (canBreak(*blockXY))
			{
				if (canDropItem(*blockXY))
					createItemMob(x, y, genericBlock(*blockXY));
				else if (!isSurvival())
					addInventory(*blockXY);
				*blockXY = AIR;
			}
			break;
	}
	updateBrightnessAround(world, x, y);
}

void placeBlock(WorldObject *world, int x, int y, bool bg)
{
	int blockID = getBlockID(getSelectedSlot());
	if (subInventory(blockID, 1))
	{
		switch (blockID)
		{
			case CHEST:
				createChest(world, x, y, bg);
				break;
			case FURNACE:
				createFurnace(world, x, y, bg);
				break;
			default:
				if (bg)
					world->bgblocks[x][y] = blockID;
				else
					world->blocks[x][y] = blockID;
				break;
		}
	}
	updateBrightnessAround(world, x, y);
}

int destroyTime(int blockID, int selectedBlock) //Calculated the required frames to destroy a block
{
	if (!isSurvival())
		return 0;
	int handHardness = (getHardness(selectedBlock) < 0) ? getHardness(selectedBlock)*-1 : 1; // Hardness of item in hand
	int blockTypeXY = getType(blockID);
	switch (getType(selectedBlock)) //Ensures tools only work for correct material
	{
		case AXE:
			if (blockTypeXY != WOOD) handHardness = 1;
			break;
		case PICKAXE:
			if (blockTypeXY != STONEBLOCK) handHardness = 1;
			break;
		case SHOVEL:
			if (blockTypeXY != SOIL) handHardness = 1;
			break;
	}
	return getHardness(blockID)*10 / handHardness;
}

void changeTarget(int newSum, int newBlock)
{
	targetSum = newSum;
	framesTillBreak = destroyTime(newBlock, getBlockID(getSelectedSlot()));
	framesOnBlock = 0;
	finishedTask = false;
}

void activateBlock(WorldObject *world, int x, int y, bool bg)
{
	switch (bg ? world->bgblocks[x][y] : world->blocks[x][y])
	{
		case AIR:
		{
			placeBlock(world, x, y, bg);
			break;
		}
		case CHEST:
			openChest(world, x, y, bg);
			break;
		case FURNACE:
			//openFurnace(world,x,y,bg);
			break;
		default:
			break;
	}
}

bool attackMob(WorldObject *world, int px, int py)
{
	int mobNum = isMobAt(px + world->camX, py + world->camY);
	if (mobNum != -1)
	{
		int damage;
		switch (getBlockID(getSelectedSlot()))
		{
			case SWORD_DIAMOND:
				damage = 6;
				break;
			case SWORD_IRON:
				damage = 4;
				break;
			case SWORD_GOLD:
				damage = 3;
				break;
			case SWORD_STONE:
				damage = 3;
				break;
			case SWORD_WOOD:
				damage = 2;
				break;
			default:
				damage = 1;
				break;
		}
		mobHandlerHurtMob(mobNum, damage, PLAYER_HURT);
		return true;
	}
	else
		return false;
}

void miningUpdate(WorldObject* world, touchPosition touch)
{
	int x = (touch.px - 1 + world->camX) / 16;
	int y = (touch.py - 1 + world->camY) / 16;
	if (!(keysHeld() & KEY_TOUCH) || miningDisabled)
	{
		if (targetSum != -1)
		{
			if (!finishedTask && framesOnBlock < ACTIVATE_DELAY)
				activateBlock(world, x, y, keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH));
			targetSum = -1;
			finishedTask = false;
		}
		return;
	}
	int touchedBlock;
	bool oldLayerIsBG = layerIsBG;
	if (world->blocks[x][y] == AIR || (isBlockWalkThrough(world->blocks[x][y])
			&& keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH)))
	{
		touchedBlock = world->bgblocks[x][y];
		layerIsBG = true;
	}
	else if (!(keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH)))
	{
		touchedBlock = world->blocks[x][y];
		layerIsBG = false;
	}
	else
		touchedBlock = AIR;
	if (targetSum != x + y || (oldLayerIsBG != layerIsBG && !finishedTask)) //Changed block
	{
		if (attackMob(world, touch.px, touch.py))
			finishedTask = true;
		else
			changeTarget(x + y, touchedBlock);
	}
	if (touchedBlock != AIR && framesOnBlock >= framesTillBreak && !finishedTask)
	{
		destroyBlock(world, x, y, world->blocks[x][y] == AIR || keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH));
		changeTarget(x + y, AIR);
		finishedTask = true;
	}
	if (touchedBlock != AIR && !finishedTask)
		++framesOnBlock;
}