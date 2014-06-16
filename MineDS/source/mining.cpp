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
#include "titlescreen.h"
#include "inventory.h"
#include "blocks.h"
#include "blockName.h"
#include "controls.h"
//#include "graphics/inventoryGraphics.h"
#include "mining.h"
bool skipLightUpdate = false; //Whether to skip light update
int selectedblock = 0;
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
int handHardness; //Hardness of item in hand
bool destroy = false;

int getSelectedblock()
{
	return selectedblock;
}

void miningSetScene(bool a)
{
	incutscene = a;
}

void calculateTopBlock()
{
	if (loadedgraphic) unloadGraphic(&topBlock);
	loadGraphicSub(&topBlock, 2, selectedblock);
	loadedgraphic = true;
}

void updateTopName()
{
	printXY(2, 7, "                            ");
	if (selectedblock != AIR)
		iprintf("\x1b[7;2H%s", getName(selectedblock));
}

void setSelectedBlock(int blockID)
{
	selectedblock = blockID;
	calculateTopBlock();
}

void blocksCanPlace()
{
	canPlaceBlocks = true;
	framecounting = 0;
}

void setBlock(worldObject* world, int x, int y, bool tap)
{
	bool change = false;
	skipLightUpdate = false;
	if (miningX != x)
	{
		miningX = x;
		change = true;
	}
	if (miningY != y)
	{
		miningY = y;
		change = true;
	}
	bool something = true;
	if ((world->blocks[x][y] == AIR && !(keysHeld() & getKey(ACTION_CROUCH)) && (selectedblock == AIR || item(selectedblock))) || (world->blocks[x][y] == AIR && world->bgblocks == AIR && (keysHeld() & getKey(ACTION_CROUCH)) && (selectedblock == AIR || item(selectedblock))))
	{
		skipLightUpdate = true;
		return;
	}
	if ((keysHeld() & getKey(ACTION_CROUCH)) || selectedblock == CACTUS)
	{
		if (world->bgblocks[x][y] != AIR) something = false;
	}
	if (world->blocks[x][y] == AIR && something && !item(selectedblock) && hasChangedBlock == false)
	{
		if ((keysHeld() & getKey(ACTION_CROUCH))) //Place in Background
		{
			if (subInventory(selectedblock, 1))
				world->bgblocks[x][y] = selectedblock;
			checkBlockPlace(x, y, world, true);
		}
		else //Place in Foreground
		{
			if (subInventory(selectedblock, 1))
				world->blocks[x][y] = selectedblock;
			checkBlockPlace(x, y, world, false);
		}
		hasChangedBlock = true;
	}
	else if (world->blocks[x][y] != AIR && hasChangedBlock == false) //Mine in Foreground
	{
		if (isSurvival())
		{
			if (change) //If new spot tapped on screen
			{
				if (getHardness(selectedblock) < 0) //If object in hand is an item (Not block)
					handHardness = getHardness(selectedblock)*-1;
				else //If item is a block
					handHardness = 1;
				switch (getType(selectedblock))
				{
					case AXE: if (getType(world->blocks[x][y]) != WOOD)
						{
							handHardness = 1;
						}
						break;
						//Using an axe on something other than wood will give the axe a hardness of 1
					case PICKAXE: if (getType(world->blocks[x][y]) != STONEBLOCK)
						{
							handHardness = 1;
						}
						break;
					case SHOVEL: if (getType(world->blocks[x][y]) != SOIL)
						{
							handHardness = 1;
						}
						break;
				}
				mining = getHardness(world->blocks[x][y])*10 / handHardness;
				miningRate = 1;
			}
			else if (!change) //Stylus is on same block and Held
				mining -= miningRate;
			miningX = x; //Previous x
			miningY = y; //Previous Y
			skipLightUpdate = true;
			if (mining <= 0)
			{
				if (addInventory(world->blocks[x][y], 1))
				{
					hasChangedBlock = true;
					world->blocks[x][y] = AIR;
					checkBlockDelete(x, y, world, false);
					skipLightUpdate = false;
				}
				miningX = -1; //Ensuring mining will be reset on next loop
				miningY = -1;
			}
		}
		else if (tap)
		{
			if (addInventory(world->blocks[x][y], 1))
			{
				world->blocks[x][y] = AIR;
				checkBlockDelete(x, y, world, false);
				hasChangedBlock = true;
			}
		}
	}
	else if (world->blocks[x][y] == AIR && (keysHeld() & getKey(ACTION_CROUCH)) && hasChangedBlock == false) //Mine in Background
	{
		if (isSurvival())
		{
			if (change) //If new spot tapped on screen
			{
				if (getHardness(selectedblock) < 0) //If object in hand is an item (Not block)
					handHardness = getHardness(selectedblock)*-1;
				else //If item is a block
					handHardness = 1;
				switch (getType(selectedblock))
				{
					case AXE: if (getType(world->bgblocks[x][y]) != WOOD)handHardness = 1;
						break;
						//Using an axe on something other than wood will give the axe a hardness of 1
					case PICKAXE: if (getType(world->bgblocks[x][y]) != STONEBLOCK) handHardness = 1;
						break;
					case SHOVEL: if (getType(world->bgblocks[x][y]) != SOIL) handHardness = 1;
						break;
					default:
					{
						if (getHardness(selectedblock) < 0) //If object in hand is an item (Not block)
							handHardness = getHardness(selectedblock)*-1;
						else //If item is a block
							handHardness = 1;
						break;
					}
				}
				mining = getHardness(world->bgblocks[x][y])*10 / handHardness;
				miningRate = 1;
			}
			else if (!change) //Stylus is on same block and Held
				mining -= miningRate;
			miningX = x; //Previous x
			miningY = y; //Previous Y
			skipLightUpdate = true;
			if (mining <= 0)
			{
				if (addInventory(world->bgblocks[x][y], 1))
				{
					hasChangedBlock = true;
					world->bgblocks[x][y] = AIR;
					checkBlockDelete(x, y, world, true);
					skipLightUpdate = false;
				}
				miningX = -1; //Ensuring mining will be reset on next loop
				miningY = -1;
			}
		}
		else if (tap)
		{
			if (addInventory(world->bgblocks[x][y], 1))
			{
				world->bgblocks[x][y] = AIR;
				checkBlockDelete(x, y, world, true);
				hasChangedBlock = true;
			}
		}
	}
	else return;
	//Send a WIFI Update now, if wifi is enabled!
	if (isWifi())
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
			mobHandlerHurtMob(mobNum, 2, PLAYER_HURT);
		}
		else if ((canPlaceBlocks && !incutscene) || destroy)
		{
			setBlock(world, x, y, true);
			if (!skipLightUpdate)
				updateBrightnessAround(world, x, y);
		}
	}
	else if (keysHeld() & KEY_TOUCH)
	{
		int x = (touch.px + a) / 16;
		int y = (touch.py + b) / 16;
		setBlock(world, x, y, false);
		if (!skipLightUpdate)
			updateBrightnessAround(world, x, y);
		/*if (skipLightUpdate)
		 *			print_message("Skip Light Update\n");
		 *		else
		 *			print_message("Light Update\n");*/
	}
	else
	{
		hasChangedBlock = false;
	}
	if (keys & getKey(ACTION_ITEM_LEFT))
	{
		selectedblock--;
		if (selectedblock < 0) selectedblock = NUM_BLOCKS;
		if (isSurvival())
		{
			while (checkInventory(selectedblock) == 0)
			{
				selectedblock--;
				if (selectedblock < 0) selectedblock = NUM_BLOCKS;
			}
		}
		calculateTopBlock();
		updateTopName();
	}
	else if (keys & getKey(ACTION_ITEM_RIGHT))
	{
		selectedblock++;
		if (selectedblock > NUM_BLOCKS) selectedblock = 0;
		if (isSurvival())
		{
			while (checkInventory(selectedblock) == 0)
			{
				selectedblock++;
				if (selectedblock > NUM_BLOCKS) selectedblock = 0;
			}
		}
		calculateTopBlock();
		updateTopName();
	}
	if (selectedblock < 0)
	{
		selectedblock = NUM_BLOCKS;
		calculateTopBlock();
	}
	else if (selectedblock > NUM_BLOCKS)
	{
		selectedblock = 0;
		calculateTopBlock();
	}
	//Draw the selected block
	if (selectedblock != 0)
	{
		showGraphic(&topBlock, 0, 48);
	}
	if (canPlaceBlocks == false) framecounting++;
	if (framecounting > 60)
	{
		//More than a second, and no block confirm?
		placeBlock(last_x, last_y);
		framecounting = 0;
		failedAttempts++;
	}
}

