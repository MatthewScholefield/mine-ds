#include <nds.h>
#include "inventoryGraphics.h"
#include "subBgHandler.h"
#include "graphics.h"
#include "../mining.h"
#include "../inventory.h"
#include "../general.h"
#include "../mainGame.h"
#include "../blockID.h"
#include "../titlescreen.h"
#include "../inventory.h"
#include "../chests.h"

Graphic invBlockGfx[NUM_INV_SPACES];
Graphic chestBlockGfx[CHEST_SLOTS];

bool empty = false; //Whether the inventory is empty
int total;
bool loadedInvGfx[NUM_INV_SPACES];
bool loadedChestGfx[NUM_INV_SPACES];
int loadedInvID[NUM_INV_SPACES] = {};
int loadedChestID[NUM_INV_SPACES] = {};
bool enabled = false, oldEnabled = true;
bool chestOpened = false;
int openedChestID;

int (*openedChestPtr)[CHEST_SLOTS][2];

void enableInvGraphics()
{
	enabled = true;
}

void disableInvGraphics()
{
	enabled = false;
}

int getOpenedChestID()
{
	return chestOpened ? openedChestID : -1;
}

int getChestBlockID(int slot)
{
	return (*openedChestPtr)[slot][INDEX_BLOCK_ID];
}

void drawBasicInv(int startX, int startY, int sizeX, int sizeY)
{
	for (int i = startX + 1; i < startX + sizeX - 1; ++i)
	{
		//Draw item boxes
		setSubBgTile(i, startY + 1, 154, (i - startX - 1) % 2);
		setSubBgTile(i, startY + 2, 154, V_FLIP + (i - startX - 1) % 2);
		setSubBgTile(i, startY + 4, 154, (i - startX - 1) % 2);
		setSubBgTile(i, startY + 5, 154, V_FLIP + (i - startX - 1) % 2);

		//Draw border
		setSubBgTile(i, startY, 30);
		setSubBgTile(i, startY + sizeY / 2, 28 + (i % 2));
		setSubBgTile(i, startY + sizeY - 1, 30, V_FLIP);
	}
	for (int j = startY + 1; j < startY + sizeY - 1; ++j)
	{
		setSubBgTile(startX, j, 27);
		setSubBgTile(startX + sizeX - 1, j, 27, H_FLIP);
	}
	//Draw Corners
	setSubBgTile(startX, startY, 26);
	setSubBgTile(startX + sizeX - 1, startY, 26, H_FLIP);
	setSubBgTile(startX, startY + sizeY - 1, 26, V_FLIP);
	setSubBgTile(startX + sizeX - 1, startY + sizeY - 1, 26, BOTH_FLIP);
}

void drawQuantity(bool chest, int startX, int startY, int amountPerRow, int numRows, int xDist, int yDist)
{
	for (int i = 0; i < amountPerRow; ++i)
		for (int j = 0; j < numRows; ++j)
			if (chest)
			{
				if (isSurvival() && (*openedChestPtr)[j * amountPerRow + i][INDEX_AMOUNT] != 0 && (*openedChestPtr)[j * amountPerRow + i][INDEX_BLOCK_ID] != 0)
					printXY(startX + i * xDist, startY + j * yDist, (*openedChestPtr)[j * amountPerRow + i][INDEX_AMOUNT]);
				else
					printXY(startX + i * xDist, startY + j*yDist, "  ");
			}
			else if (!chest)
			{
				if (isSurvival() && getBlockAmount(j * amountPerRow + i) != 0 && getBlockID(j * amountPerRow + i) != 0)
					printXY(startX + i * xDist, startY + j * yDist, getBlockAmount(j * amountPerRow + i));
				else
					printXY(startX + i * xDist, startY + j * yDist, "  ");
			}
}

void updateInvGraphics()
{
	oldEnabled = enabled;
	if (!enabled)
		return;
	drawBasicInv(0, 8, 32, 7);
	drawQuantity(false, 1, 10, 15, 2, 2, 3);
}

void drawGraphics(bool chest, int startX, int startY, int amountPerRow, int numRows, int xDist, int yDist)
{
	int a = 0;
	for (int j = 0; j < numRows; ++j)
		for (int i = 0; i < amountPerRow; ++i)
		{
			if (chest)
			{
				if ((*openedChestPtr)[a][INDEX_BLOCK_ID] == AIR && loadedChestGfx[a])
				{
					unloadGraphic(&chestBlockGfx[a]);
					loadedChestGfx[a] = false;
					loadedChestID[a] = AIR;
				}
				else if ((*openedChestPtr)[a][INDEX_BLOCK_ID] != loadedChestID[a])
				{
					if (loadedChestGfx[a])
						unloadGraphic(&chestBlockGfx[a]);
					else
						loadedChestGfx[a] = true;
					loadGraphicSub(&chestBlockGfx[a], 2, (*openedChestPtr)[a][INDEX_BLOCK_ID]);
					loadedChestID[a] = (*openedChestPtr)[a][INDEX_BLOCK_ID];
				}
				if (loadedChestGfx[a])
					showGraphic(&chestBlockGfx[a], startX * 8 + i * 8 * xDist, startY * 8 + j * 8 * yDist, false, 2);

			}
			else if (!chest)
			{
				if (getBlockID(a) == AIR && loadedInvGfx[a])
				{
					unloadGraphic(&invBlockGfx[a]);
					loadedInvGfx[a] = false;
					loadedInvID[a] = AIR;
				}
				else if (getBlockID(a) != loadedInvID[a])
				{
					if (loadedInvGfx[a])
						unloadGraphic(&invBlockGfx[a]);
					else
						loadedInvGfx[a] = true;
					loadGraphicSub(&invBlockGfx[a], 2, getBlockID(a));
					loadedInvID[a] = getBlockID(a);
				}
				if (loadedInvGfx[a])
					showGraphic(&invBlockGfx[a], startX * 8 + i * 8 * xDist, startY * 8 + j * 8 * yDist, false, 2);
			}
			++a;
		}
}

void drawInv() //Draws the items in the inventory (called by the mainPlayer->update if in survival mode)
{
	if (!enabled)
	{
		oldEnabled = false;
		return;
	}
	if (!oldEnabled)
	{
		updateInvGraphics();
		if (chestOpened)
			updateChestItems();
		oldEnabled = true;
	}
	if (chestOpened)
		drawGraphics(true, 1, 1, 15, 2, 2, 3);
	drawGraphics(false, 1, 9, 15, 2, 2, 3);
}

void openChest(WorldObject *world, int x, int y, bool bg)
{
	if (chestOpened) //Another chest is already opened
		return;
	chestOpened = true;
	openedChestID = getChestID(world, x, y, bg);
	openedChestPtr = &world->chests[openedChestID];
	for (int i = 0; i < CHEST_SLOTS; ++i)
		loadedChestGfx[i] = false;
	updateChestItems();
	openInventory();
}

void closeChest()
{
	if (!chestOpened)
		return;
	chestOpened = false;
	for (int i = 0; i < CHEST_SLOTS; ++i)
	{
		if (loadedChestGfx[i])
			unloadGraphic(&chestBlockGfx[i]);
		loadedChestID[i] = AIR;
		loadedChestGfx[i] = false;
	}
	drawBackground();
	updateInvGraphics();
	drawInvButtons(true, isSurvival());
}

void updateChestItems() //Changes graphics and text
{
	oldEnabled = enabled;
	if (!enabled)
		return;
	drawBasicInv(0, 0, 32, 7);
	drawQuantity(true, 1, 2, 15, 2, 2, 3);
}