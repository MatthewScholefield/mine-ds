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
#include "../blockName.h"

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

void drawSlots(int selectedSlot, int startX, int startY, int xCount, int yCount, int xSpace, int ySpace)
{
	int slot = 0;
	for (int j = 0; j < yCount; ++j)
		for (int i = 0; i < xCount; ++i)
		{
			int tile = 154;
			if (slot == selectedSlot)
				tile = 155;
			setSubBgTile(startX + i*xSpace, startY + j*ySpace, tile);
			setSubBgTile(startX + i*xSpace, startY + j * ySpace + 1, tile, V_FLIP);
			setSubBgTile(startX + i * xSpace + 1, startY + j*ySpace, tile, H_FLIP);
			setSubBgTile(startX + i * xSpace + 1, startY + j * ySpace + 1, tile, BOTH_FLIP);
			++slot;
		}
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

void drawSelectedFrame()
{
	setSubBgTile(0, 7, 27);
	setSubBgTile(3, 7, 27, H_FLIP);
	setSubBgTile(0, 6, 27);
	setSubBgTile(3, 6, 27, H_FLIP);
	setSubBgTile(0, 5, 26);
	setSubBgTile(3, 5, 26, H_FLIP);
	setSubBgTile(1, 5, 30);
	setSubBgTile(2, 5, 30);
	setSubBgTile(0, 8, 27);
	setSubBgTile(0, 8, 27);
	setSubBgTile(3, 8, 27, H_FLIP);
	setSubBgTile(3, 8, 28);
	setSubBgTile(1, 8, 28);
	setSubBgTile(2, 8, 29);
	for (int i = 6; i < 8; ++i)
		for (int j = 1; j < 3; ++j)
			setSubBgTile(j, i, ((i % 2) ? 90 : 122) + j % 2);
}

void updateInvGraphics()
{
	oldEnabled = enabled;
	if (!enabled)
		return;
	drawBoxFrame(0, 8, 32, 7);
	drawBoxCenter(1, 11, 30, 1);
	drawSlots(getSelectedSlot(), 1, 9);
	if (getOpenedChestID() == -1)
		drawSelectedFrame();
	else
	{
		setSubBgTile(0, 6, 27);
		setSubBgTile(0, 7, 27);
		setSubBgTile(0, 8, 27);
		setSubBgTile(3, 6, 28);
		setSubBgTile(3, 7, 27, H_FLIP);
		setSubBgTile(3, 8, 28);
		drawBoxCenter(1, 6, 2, 3);
	}
	drawQuantity(false, 1, 10, 15, 2, 2, 3);
	updateTopName(getBlockID(getSelectedSlot()));
	changeInvSelectedGraphic();
}

void refreshInventoryGraphics()
{
	for (int i = 0; i < NUM_INV_SPACES; ++i)
		loadedInvID[i] = AIR;
	for (int i = 0; i < CHEST_SLOTS; ++i)
		loadedChestID[i] = AIR;
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
		if (chestOpened)
			updateChestItems();
		updateInvGraphics();
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
	updateInvGraphics();
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
	if (!chestOpened)
		return;
	drawBoxFrame(0, 0, 32, 7);
	drawBoxCenter(1, 3, 30, 1);
	if (getSelectedSlot()<-1)
		drawSlots(-getSelectedSlot() - 2, 1, 1);
	else
		drawSlots(-1, 1, 1);
	drawQuantity(true, 1, 2, 15, 2, 2, 3);
}