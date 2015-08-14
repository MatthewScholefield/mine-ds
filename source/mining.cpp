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
#include "blockPages.h"

bool miningDisabled = false;
int framesOnBlock;
int framesTillBreak;
int targetSum = -1;
unsigned char soundOffset;
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

void destroyBlock(WorldObject *world, int x, int y, bool bg, bool byHand)
{
	int blockID = getBlockID(getSelectedSlot());
	int *blockXY = bg ? &world->bgblocks[x][y] : &world->blocks[x][y];
	if (*blockXY == AIR)
		return;
	switch (*blockXY)
	{
	case CHEST:
		destroyChest(world, x, y, bg);
		break;
	case FURNACE:
		destroyFurnace(world, x, y, bg);
		break;
	case WATER:
		if (blockID == BUCKET_EMPTY)
		{
			*blockXY = AIR;
			subInventory(blockID,1);
			addInventory(BUCKET_WATER);
		}
		break;
	default:
		if (!byHand || canBreak(*blockXY))
		{
			if (!byHand || canDropItem(*blockXY))
				createItemMob(x, y, getSurvivalItem(*blockXY));
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
	if (item(blockID) && !blockItem(blockID))
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
		case BUCKET_WATER:
			createWaterMob(x, y);
			addInventory(BUCKET_EMPTY);
			break;
		default:
			if (!item(blockID))
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
	int val = getHardness(blockID)*10 / handHardness;
	if (val > ACTIVATE_DELAY)
		return val;
	else
		return ACTIVATE_DELAY;
}

void changeTarget(int newSum, int newBlock)
{
	targetSum = newSum;
	framesTillBreak = destroyTime(newBlock, getBlockID(getSelectedSlot()));
	framesOnBlock = 0;
	finishedTask = false;
	soundOffset = getTime() % HIT_SOUND_DELAY;
}

void activateBlock(WorldObject *world, int x, int y, bool bg)
{
	switch (bg ? world->bgblocks[x][y] : world->blocks[x][y])
	{
	case AIR:
	{
		placeBlock(world, x, y, bg);
		playBlockSfx(bg ? world->bgblocks[x][y] : world->blocks[x][y], SOUND_TYPE_PLACE, 255, getBlockPanning(x, world->camX));
		break;
	}
	case CHEST:
		openChest(world, x, y, bg);
		break;
	case FURNACE:
		//openFurnace(world,x,y,bg);
		break;
	case DOOR_OPEN_BOTTOM:
		--y;
	case DOOR_OPEN_TOP:
	{
		int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
		int &blockBelowXY = bg ? world->bgblocks[x][y + 1] : world->blocks[x][y + 1];
		blockXY = DOOR_CLOSED_TOP;
		blockBelowXY = DOOR_CLOSED_BOTTOM;
		playSound(SOUND_DOOR_CLOSE, 255, getBlockPanning(x, world->camX));
		break;
	}
	case DOOR_CLOSED_BOTTOM:
		--y;
	case DOOR_CLOSED_TOP:
	{
		int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
		int &blockBelowXY = bg ? world->bgblocks[x][y + 1] : world->blocks[x][y + 1];
		blockXY = DOOR_OPEN_TOP;
		blockBelowXY = DOOR_OPEN_BOTTOM;
		playSound(SOUND_DOOR_OPEN, 255, getBlockPanning(x, world->camX));
		break;
	}
	default:
		if (!isSurvival())
			destroyBlock(world, x, y, bg);
		break;
	}
}

bool attackMob(WorldObject *world, int px, int py)
{
	BaseMob_ptr targetMob = isMobAt(px + world->camX, py + world->camY);
	if (targetMob != nullptr)
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
		targetMob->hurt(damage, PLAYER_HURT);
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
	if ((world->blocks[x][y] == AIR && isSurvival()) || (isBlockWalkThrough(world->blocks[x][y])
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
	if (touchedBlock != AIR && framesOnBlock >= framesTillBreak && !finishedTask && framesOnBlock >= ACTIVATE_DELAY)
	{
		destroyBlock(world, x, y, world->blocks[x][y] == AIR || keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH));
		changeTarget(x + y, AIR);
		finishedTask = true;
		playBlockSfx(touchedBlock, SOUND_TYPE_DESTROY, 255, touch.px);
	}
	if (touchedBlock != AIR && !finishedTask)
	{
		++framesOnBlock;
		if ((getTime() - soundOffset) % HIT_SOUND_DELAY == 0)
			playBlockSfx(touchedBlock, SOUND_TYPE_DESTROY, 80, touch.px);
	}

}
