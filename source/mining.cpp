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
	if (item(blockID))
		return;
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
        if(!item(blockId))
        {
  				if (bg)
  					world->bgblocks[x][y] = blockID;
  				else
  					world->blocks[x][y] = blockID;
        }
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
