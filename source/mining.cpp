#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include <nds.h>
#include <stdlib.h>
#include <stdio.h>
#include "mobs/hurt.h"
#include "mobs/mobHandler.h"
#include "graphics/graphics.h"
#include "communications.h"
#include "general.h"
#include "nifi.h"
#include "blockupdate.h"
#include "mainGame.h"
#include "inventory.h"
#include "blocks.h"
#include "blockName.h"
#include "Config.h"
#include "mining.h"

bool skipLightUpdate = false; //Whether to skip light update
int invSlot = 0;
bool loadedTopGraphic = false;
Graphic topBlock;
bool disableTouchMine = false;
bool canPlaceBlocks = true;
bool hasChangedBlock = false;
int frameCounting = 0;
int failedAttempts = 0;
int lastX, lastY;
int miningX = -1; //X,Y Value of block that is currently being mined
int miningY = -1;
int mining = 10; //Length till block break
int miningRate = 1; //Rate at which Block is Mined. Caculated from item in hand and block's hardness. Currently not calculated

int getSelectedSlot()
{
	return invSlot;
}

void setMiningDisabled(bool set)//false enables block destroy, true disable it
{
	disableTouchMine = set;
}

void calculateTopBlock()
{
	if (loadedTopGraphic) unloadGraphic(&topBlock);
	loadGraphicSub(&topBlock, 2, getBlockID(invSlot));
	loadedTopGraphic = true;
}

void updateTopName()
{
	printXY(2, 7, "                            ");
	if (getBlockID(invSlot) != AIR)
		iprintf("\x1b[7;2H%s", getName(getBlockID(invSlot)));
}

void setSelectedSpace(int space)
{
	invSlot = space;
	calculateTopBlock();
}

void blocksCanPlace()
{
	canPlaceBlocks = true;
	frameCounting = 0;
}

void mineBlock(worldObject* world, int x, int y, bool bg)
{
	int selectedBlock = getBlockID(invSlot);
	if (getType(selectedBlock) == SWORD)
	{
		skipLightUpdate = true;
		return; //Swords don't mine blocks
	}
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	bool stylusMoved = (miningX == x || miningY == y) ? false : true;
	if (isSurvival())
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
		}
		else
			mining -= miningRate;
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
		{
			createItemMob(x, y, genericBlock(blockXY));
		}
		else if (!isSurvival())
			addInventory(blockXY);
		blockXY = AIR;
		hasChangedBlock = true;
	}
}

void setBlock(worldObject* world, int x, int y)
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

void miningUpdate(worldObject* world, int a, int b, touchPosition touch, int keys) // keys = keysDown();
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
	else
		hasChangedBlock = false;
	if (keys & getGlobalSettings()->getKey(ACTION_ITEM_LEFT))
	{
		int origSlot = invSlot;
		--invSlot;
		if (invSlot < 0) invSlot = NUM_INV_SPACES - 1;
		while ((checkInventorySlot(invSlot) == 0 || getBlockID(invSlot) == AIR) && invSlot != origSlot)
		{
			--invSlot;
			if (invSlot < 0) invSlot = NUM_INV_SPACES - 1;
		}
		calculateTopBlock();
		updateTopName();
	}
	else if (keys & getGlobalSettings()->getKey(ACTION_ITEM_RIGHT))
	{
		int origSlot = invSlot;
		++invSlot;
		if (invSlot < 0) invSlot = NUM_INV_SPACES - 1;
		while ((checkInventorySlot(invSlot) == 0 || getBlockID(invSlot) == AIR) && invSlot != origSlot)
		{
			++invSlot;
			if (invSlot >= NUM_INV_SPACES) invSlot = 0;
		}
		calculateTopBlock();
		updateTopName();
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
}

