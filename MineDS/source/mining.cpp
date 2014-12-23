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
int selectedBlock = 0;
bool loadedgraphic = false;
Graphic topBlock;
bool incutscene = false;
bool canPlaceBlocks = true;
bool hasChangedBlock = false;
int framecounting = 0;
int failedAttempts = 0;
int last_x, last_y;
int miningX = -1; //X,Y Value of block that is currently being mined
int miningY = -1;
int mining = 10; //Length till block break
int miningRate = 1; //Rate at which Block is Mined. Caculated from item in hand and block's hardness. Currently not calculated

int getSelectedblock()
{
	return selectedBlock;
}

void miningSetScene(bool a) //false enables block destroy, true disable it
{
	incutscene = a;
}

void calculateTopBlock()
{
	if (loadedgraphic) unloadGraphic(&topBlock);
	loadGraphicSub(&topBlock, 2, selectedBlock);
	loadedgraphic = true;
}

void updateTopName()
{
	printXY(2, 7, "                            ");
	if (selectedBlock != AIR)
		iprintf("\x1b[7;2H%s", getName(selectedBlock));
}

void setSelectedBlock(int blockID)
{
	selectedBlock = blockID;
	calculateTopBlock();
}

void blocksCanPlace()
{
	canPlaceBlocks = true;
	framecounting = 0;
}

void mineBlock(worldObject* world, int x, int y, bool bg)
{
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
				case AXE: if (blockTypeXY != WOOD) handHardness = 1;
					break;
				case PICKAXE: if (blockTypeXY != STONEBLOCK) handHardness = 1;
					break;
				case SHOVEL: if (blockTypeXY != SOIL) handHardness = 1;
					break;
			}
			mining = getHardness(blockXY)*10 / handHardness;
			miningRate = 1;
		}
		else
		{
			mining -= miningRate;
		}
		if (mining > 0)
		{
			skipLightUpdate = true;
			return;
		}
		miningX = -1;
		miningY = -1;
	}
	if (addInventory(blockXY, 1))
	{
		blockXY = AIR;
		checkBlockDelete(x, y, world, bg);
		hasChangedBlock = true;
	}
}

void setBlock(worldObject* world, int x, int y)
{
	skipLightUpdate = false;
	bool isCrouched = (keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH));
	bool isPlaceableBlock = (selectedBlock != AIR && !item(selectedBlock) && (!hasChangedBlock || getGlobalSettings()->getProperty(PROPERTY_DRAW)));
	if (isCrouched) // Background
	{
		skipLightUpdate = true; //Background does not affect light
		if (world->blocks[x][y] != AIR) // Foreground not empty don't place or mine
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
					world->bgblocks[x][y] = selectedBlock;
				checkBlockPlace(x, y, world, isCrouched);
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
					world->blocks[x][y] = selectedBlock;
				checkBlockPlace(x, y, world, isCrouched);
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
		last_x = x;
		last_y = y;
		placeBlock(x, y);
	}
}

void miningUpdate(worldObject* world, int a, int b, touchPosition touch, int keys) // keys = keysDown();
{
	if (incutscene)
		return;
	if (keys & KEY_TOUCH)
	{
		int x = (touch.px + a) / 16;
		int y = (touch.py + b) / 16;
		int mobNum = isMobAt(touch.px + a, touch.py + b);
		if (mobNum != -1)
		{
			int damage;
			switch (selectedBlock)
			{
				case SWORD_DIAMOND:damage = 6;
					break;
				case SWORD_IRON:damage = 4;
					break;
				case SWORD_GOLD:damage = 3;
					break;
				case SWORD_STONE:damage = 3;
					break;
				case SWORD_WOOD: damage = 2;
					break;
				default: damage = 1;
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
	{
		hasChangedBlock = false;
	}
	if (keys & getGlobalSettings()->getKey(ACTION_ITEM_LEFT))
	{
		--selectedBlock;
		if (selectedBlock < 0) selectedBlock = NUM_BLOCKS;
		if (isSurvival())
		{
			while (checkInventory(selectedBlock) == 0)
			{
				--selectedBlock;
				if (selectedBlock < 0) selectedBlock = NUM_BLOCKS;
			}
		}
		calculateTopBlock();
		updateTopName();
	}
	else if (keys & getGlobalSettings()->getKey(ACTION_ITEM_RIGHT))
	{
		++selectedBlock;
		if (selectedBlock > NUM_BLOCKS) selectedBlock = 0;
		if (isSurvival())
		{
			while (checkInventory(selectedBlock) == 0)
			{
				++selectedBlock;
				if (selectedBlock > NUM_BLOCKS) selectedBlock = 0;
			}
		}
		calculateTopBlock();
		updateTopName();
	}
	if (selectedBlock < 0)
	{
		selectedBlock = NUM_BLOCKS;
		calculateTopBlock();
	}
	else if (selectedBlock > NUM_BLOCKS)
	{
		selectedBlock = 0;
		calculateTopBlock();
	}
	//Draw the selected block
	if (selectedBlock != 0)
	{
		showGraphic(&topBlock, 0, 48);
	}
	if (canPlaceBlocks == false) ++framecounting;
	if (framecounting > 60)
	{
		//More than a second, and no block confirm?
		placeBlock(last_x, last_y);
		framecounting = 0;
		++failedAttempts;
	}
}

