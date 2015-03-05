/*
 * -----------------------------------------
 * |             Inventory.cpp             |
 * |           Inventory Funtions          |
 * -----------------------------------------
 */
#include <stdio.h>
#include "mining.h" //for NUM_BLOCKS
#include "graphics/graphics.h"
#include "graphics/inventoryGraphics.h"
#include "graphics/subBgHandler.h"
#include "graphics/Button.h"
#include "titlescreen.h"
#include "crafting.h"
#include "inventory.h"
#include "blockID.h"
#include "blocks.h"
#include "general.h"
#include "mainGame.h" // for isSurvival
#include <nds.h> // for keysDown()
#include <stdio.h> // for fileio
#include "Config.h"
#include "world.h"
#include "files.h"
#include "graphics/Button.h"
#include "blockPages.h"
#include "blockName.h"
bool loadedGraphic = false;
int selectedspace = -1;
Graphic heldBlock;
Button backButton(1, 16, "Back", false);
Button saveButton(8, 16, "Save World", false);
Button craftButton(21, 16, "Crafting", false);
Button pageButton(21, 16, "Pages", 9, false);
unsigned char oldX, oldY;
/*					A reminder:
 * INVENTORY STRUCT USES blockId, not blockID, for what ever reason!!!!!
 */


int showingInventory;
Inventory mainPlayerInv;

void drawInvButtons(bool drawBack, bool survival)
{
	if (drawBack) backButton.draw();
	saveButton.draw();
	if (survival)
		craftButton.draw();
	else
		pageButton.draw();
}

void saveInventory(FILE* data)
{
	for (int i = 0; i < NUM_INV_SPACES; ++i)
		fprintf(data, "%d %d ",mainPlayerInv.blocks[i].blockId, mainPlayerInv.blocks[i].blockAmount);
}

int getInventoryState()
{
	return showingInventory;
}

int spaceForItem(int blockID)
{
	int space = -1;
	for (int i = 0; i < NUM_INV_SPACES; ++i)
	{
		//Found the correct block with correct id.
		if (mainPlayerInv.blocks[i].blockId == blockID)
		{
			space = i;
			break;
			//Skip other inventory spaces...
		}
		//Found a sutiable air space.
		else if (mainPlayerInv.blocks[i].blockId == AIR)
		{
			//Set the space as last resort, try to find correct inventory space.
			space = i;
		}
	}
	if (space == -1)
		printLocalMessage("No space for item");
	return space;
}

int genericBlock(int blockID)
{
	switch (blockID)
	{
		case GRASS:
		case JUNGLE_GRASS:
		case SNOW_GRASS:
		case MYCELIUM:
			return DIRT;
			break;
		case TALL_GRASS:
			return SEEDS_WHEAT;
			break;
		case BEDROCK:
			return false;
			break; //Cannot break bedrock
		case SNOW_TOP:
			return AIR;
			break; //Can break snow tops, just they won't be added to the inventory
		case MUSHROOM_STEM:
		case MUSHROOM_TOP:
			return AIR;
			break;
		case COAL_ORE:
				return COAL;
				break; //Any pickaxe can break coal
		case STONE:
			return COBBLESTONE;
			break;
		case IRON_ORE:
			return INGOT_IRON;
			break;
		case GOLD_ORE:
			return INGOT_GOLD;
			break;
		case DIAMOND_ORE:
			return DIAMOND;
			break;
	}
	return blockID;
}

void addInventory(int blockID, int amount, bool direct) //adds the specified amount to a blockvalue
{
	if (blockID == AIR)
		return;
	int space = spaceForItem(blockID);
	if (space == -1)
		return;
	mainPlayerInv.blocks[space].blockId = blockID;
	mainPlayerInv.blocks[space].blockAmount += amount;
	updateInvGraphics();
}

void addInventory(int blockID)
{
	addInventory(blockID, 1);
}

bool subInventory(int blockID, int amount) //subtracts the specified amount to a blockvalue
{
	if (isSurvival() == false) //Always return true in creative!
		return true;
	int i;
	int space = -1;
	for (i = 0; i < NUM_INV_SPACES; ++i)
	{
		//Found the correct block with correct id.
		if (mainPlayerInv.blocks[i].blockId == blockID)
		{
			space = i;
			break;
			//Skip other inventory spaces...
		}
	}
	if (space == -1)
	{
		printLocalMessage("No item to remove\n");
		return false;
	}
	mainPlayerInv.blocks[space].blockAmount -= amount;
	if (mainPlayerInv.blocks[space].blockAmount < 0)
	{
		mainPlayerInv.blocks[space].blockAmount += amount;
		printLocalMessage("Not enough items to remove\n");
		return false;
	}
	else if (mainPlayerInv.blocks[space].blockAmount == 0)
		mainPlayerInv.blocks[space].blockId = AIR;
	updateInvGraphics();
	return true;
}

int getInventorySlot(int blockID) //returns slot of blockid in inventory
{
	int space = -1;
	for (int i = 0; i < NUM_INV_SPACES; ++i)
	{
		//Found the correct block with correct id.
		if (mainPlayerInv.blocks[i].blockId == blockID)
		{
			space = i;
			break;
		}
	}
	return space;
}

int checkInventory(int blockID) //returns quantity of blockid in inventory
{
	if (blockID == AIR)
		return 64;
	int space = getInventorySlot(blockID);
	if (space == -1)
		return 0;
	return mainPlayerInv.blocks[space].blockAmount;
}

int checkInventorySlot(int slot)
{
	return mainPlayerInv.blocks[slot].blockAmount;
}

int getBlockID(int invSlot)
{
	return mainPlayerInv.blocks[invSlot].blockId;
}

int getBlockAmount(int invSlot)
{
	return mainPlayerInv.blocks[invSlot].blockAmount;
}

void clearInventory() //clears inventory
{
	int i;
	for (i = 0; i < NUM_INV_SPACES; ++i)
	{
		mainPlayerInv.blocks[i].blockAmount = 0;
		mainPlayerInv.blocks[i].blockId = 0;
	}
	updateInvGraphics();
	showingInventory = 0;
}

void changeGraphic(int blockID)
{
	if (loadedGraphic == true)
	{
		unloadGraphic(&heldBlock);
		loadedGraphic = false;
	}
	loadGraphicSub(&heldBlock, 2, blockID);
	loadedGraphic = true;
}

void updateInventory(touchPosition touch, worldObject* world, uint oldKeys)
{

	if (showingInventory == 0)
	{
		if (keysDown() & getGlobalSettings()->getKey(ACTION_SWITCH_SCREEN))
		{
			lcdMainOnTop();
			showingInventory = 1;
			miningSetScene(true);
			drawBackground();
			changeGraphic(AIR);
			backButton.setVisible(true);
			saveButton.setVisible(true);
			craftButton.setVisible(isSurvival());
			pageButton.setVisible(!isSurvival());
		}
	}
	else if (showingInventory == 1)
	{
		showGraphic(&heldBlock, 0, 0, false, 0);
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			backButton.setColored(backButton.isTouching(touch.px, touch.py));
			saveButton.setColored(saveButton.isTouching(touch.px, touch.py));
			craftButton.setColored(craftButton.isTouching(touch.px, touch.py));
			pageButton.setColored(pageButton.isTouching(touch.px, touch.py));
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH) //Release
		{
			if (oldX > 8 && oldY > 72 && oldY < 120 && oldX < 248)
			{

				int space = 0;
				space += 15 * ((oldY - 72) / 24);
				space += (oldX - 8) / 16;
				if (selectedspace != -1 && !(mainPlayerInv.blocks[selectedspace].blockId == AIR))
				{
					//Swap the blocks.
					int tmpId, tmpAmount = 0;
					tmpId = mainPlayerInv.blocks[selectedspace].blockId;
					tmpAmount = mainPlayerInv.blocks[selectedspace].blockAmount;
					mainPlayerInv.blocks[selectedspace].blockId = mainPlayerInv.blocks[space].blockId;
					mainPlayerInv.blocks[selectedspace].blockAmount = mainPlayerInv.blocks[space].blockAmount;
					mainPlayerInv.blocks[space].blockId = tmpId;
					mainPlayerInv.blocks[space].blockAmount = tmpAmount;
					selectedspace = -1;
					setSelectedSpace(getInventorySlot(AIR));
					changeGraphic(AIR);
				}
				else
				{
					selectedspace = space;
					setSelectedSpace(space);
					changeGraphic(mainPlayerInv.blocks[space].blockId);
				}
			}
			else if (backButton.isColored && backButton.isTouching(oldX, oldY))//(touch. px > (2 - 1)*8 && oldX < (2 + 5)*8 && oldY > (17 - 1)*8 && oldY < (17 + 2)*8)
			{
				selectedspace = -1;
				lcdMainOnBottom();
				showingInventory = 0;
				miningSetScene(false);
				backButton.setVisible(false);
				saveButton.setVisible(false);
				craftButton.setVisible(false);
				pageButton.setVisible(false);
				consoleClear();
				drawBackground();
				drawInvButtons(false, isSurvival());
				updateInvGraphics();
				if (!isSurvival())
					printXY(1, 15, getPageName(getBlockPage()));
			}
			else if (craftButton.isColored && craftButton.isTouching(oldX, oldY))
			{ //Crafting Menu
				backButton.setVisible(false);
				saveButton.setVisible(false);
				craftButton.setVisible(false);
				pageButton.setVisible(false);
				craftingMenuInit();
				showingInventory = 2;
			}
			else if (saveButton.isColored && saveButton.isTouching(oldX, oldY))
			{ //Save Game
				if (saveWorld(world))
					printLocalMessage("Saved Game\n");
				else
					printLocalMessage("Failed to Save Game\n");
				saveButton.setColored(false);
			}
			else if (pageButton.isColored && pageButton.isTouching(oldX, oldY))
			{ //Page Menu
				backButton.setVisible(false);
				saveButton.setVisible(false);
				craftButton.setVisible(false);
				pageButton.setVisible(false);
				pageMenuInit();
				showingInventory = 2;
			}
			else
			{
				selectedspace = -1;
				setSelectedSpace(getInventorySlot(AIR));
				changeGraphic(AIR);
				backButton.setColored(false);
				saveButton.setColored(false);
				craftButton.setColored(false);
				pageButton.setVisible(false);
			}
		}
		if (keysDown() & getGlobalSettings()->getKey(ACTION_SWITCH_SCREEN))
		{
			backButton.setVisible(false);
			saveButton.setVisible(false);
			craftButton.setVisible(false);
			pageButton.setVisible(false);
			selectedspace = -1;
			lcdMainOnBottom();
			showingInventory = 0;
			miningSetScene(false);
			consoleClear();
			drawBackground();
			updateInvGraphics();
			drawInvButtons(false, isSurvival());
			if (!isSurvival())
				printXY(1, 15, getPageName(getBlockPage()));
		}
		oldX = touch.px;
		oldY = touch.py;
	}
	else if (showingInventory == 2)
	{
		if ((isSurvival() && craftingMenuUpdate(&touch, &oldX, &oldY, &oldKeys)) || (!isSurvival() && pageMenuUpdate(&touch, &oldX, &oldY, &oldKeys)))
		{ //Leaving crafting/page menu
			showingInventory = 1;
			miningSetScene(true);
			drawBackground();
			consoleClear();
			changeGraphic(AIR);
			backButton.setVisible(true);
			saveButton.setVisible(true);
			craftButton.setVisible(isSurvival());
			pageButton.setVisible(!isSurvival());
			enableInvGraphics();
			if (!isSurvival())
				printXY(1, 15, getPageName(getBlockPage()));
		}
		else
		{
			oldX = touch.px;
			oldY = touch.py;
		}
	}
	drawInv();
}

void loadInventory(FILE* data)
{
	clearInventory();
	for (int i = 0; i < NUM_INV_SPACES; ++i)
	{
		int id, quantity;
		fscanf(data, "%d %d ",&id, &quantity);
		addInventory(id, quantity, true);
	}
}