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
#include "graphics/UI.h"
#include "inventory.h"
#include "blockID.h"
#include "blocks.h"
#include "general.h"
#include "mainGame.h" // for isSurvival
#include <nds.h> // for keysDown()
#include "Config.h"
#include "world.h"
#include "files.h"
#include "graphics/Button.h"
#include "blockPages.h"
#include "blockName.h"
#include "chests.h"
#include "interfaces/interfaceHandler.h"
#include "interfaces/InventoryInterface.h"
bool loadedTopGraphic = false;
int oldInvSlot;
Graphic selectedGraphic;
int loadedGraphic;
Button backButton(1, 16, "Back", false);
Button saveButton(8, 16, "Save World", false);
Button craftButton(21, 16, "Crafting", false);
Button pageButton(21, 16, "Pages", 9, false);

int showingInventory;
Inventory mainPlayerInv;

/*					A reminder:
 * INVENTORY STRUCT USES blockId, not blockID, for what ever reason!!!!!
 */
Inventory &getInventoryRef()
{
	return mainPlayerInv;
}

void changeInvSelectedGraphic(int blockID)
{
	if (loadedTopGraphic)
		unloadGraphic(&selectedGraphic);
	if (blockID == -1)
	{
		if (mainPlayerInv.hand<-1 && getOpenedChestID() != -1)
		{
			loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, getChestBlockID(-mainPlayerInv.hand - 2));
			drawSlots(-mainPlayerInv.hand - 2, 1, 1);
			drawSlots(mainPlayerInv.hand, 1, 9);
			loadedGraphic = getChestBlockID(-mainPlayerInv.hand - 2);
		}
		else if (mainPlayerInv.hand >= 0)
		{
			loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, getBlockID(mainPlayerInv.hand));
			drawSlots(mainPlayerInv.hand, 1, 9);
			if (getOpenedChestID() != -1)
				drawSlots(-1, 1, 1);
			loadedGraphic = getBlockID(mainPlayerInv.hand);
		}
		else
		{
			loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, AIR);
			drawSlots(-1, 1, 9);
			if (getOpenedChestID() != -1)
				drawSlots(-1, 1, 1);
			loadedGraphic = AIR;
		}
		updateTopName(getBlockID(mainPlayerInv.hand));
	}
	else
	{
		loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, blockID);
		updateTopName(blockID);
		loadedGraphic = blockID;
	}
	loadedTopGraphic = true;
}

int getHand()
{
	return mainPlayerInv.hand;
}

void setHand(int hand)
{
	mainPlayerInv.hand = hand;
}

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
		fprintf(data, "%d %d ", mainPlayerInv.blocks[i].blockId, mainPlayerInv.blocks[i].blockAmount);
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
		if (mainPlayerInv.blocks[i].blockId == blockID && mainPlayerInv.blocks[i].blockAmount < 64)
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

bool addInventory(int blockID, int amount, bool direct) //adds the specified amount to a blockvalue
{
	if (blockID == AIR)
		return false;
	int space = spaceForItem(blockID);
	if (space == -1)
		return false;
	mainPlayerInv.blocks[space].blockId = blockID;
	if (isSurvival())
		mainPlayerInv.blocks[space].blockAmount += amount;
	else
		mainPlayerInv.blocks[space].blockAmount = 1;
	InventoryInterface::triggerUpdate();
	return true;
}

bool addInventory(int blockID)
{
	return addInventory(blockID, 1);
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
		return false;
	mainPlayerInv.blocks[space].blockAmount -= amount;
	if (mainPlayerInv.blocks[space].blockAmount < 0)
	{
		mainPlayerInv.blocks[space].blockAmount += amount;
		return false;
	}
	else if (mainPlayerInv.blocks[space].blockAmount == 0)
		mainPlayerInv.blocks[space].blockId = AIR;
	InventoryInterface::triggerUpdate();
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

int getBlockID(int slot)
{
	return slot < 0 ? AIR : mainPlayerInv.blocks[slot].blockId;
}

int getBlockAmount(int slot)
{
	return slot < 0 ? 0 : mainPlayerInv.blocks[slot].blockAmount;
}

void setBlockID(int slot, int ID)
{
	if (slot < 0)
		return;
	mainPlayerInv.blocks[slot].blockId = ID;
}

void setBlockAmount(int slot, int amount)
{
	if (slot < 0)
		return;
	mainPlayerInv.blocks[slot].blockAmount = amount;
}

void clearInventory(bool direct) //clears inventory
{
	int i;
	for (i = 0; i < NUM_INV_SPACES; ++i)
	{
		mainPlayerInv.blocks[i].blockAmount = 0;
		mainPlayerInv.blocks[i].blockId = 0;
	}
	showingInventory = 0;
}

void openInventory()
{
	oldInvSlot = mainPlayerInv.hand;
	mainPlayerInv.hand = -1;
	changeInvSelectedGraphic();
	lcdMainOnTop();
	showingInventory = 1;
	setMiningDisabled(true);
	backButton.setVisible(true);
	saveButton.setVisible(true);
	craftButton.setVisible(isSurvival());
	pageButton.setVisible(!isSurvival());
	enableInvGraphics();
}

void closeInventory()
{
	if (mainPlayerInv.hand == -1)
		mainPlayerInv.hand = oldInvSlot;
	closeChest();
	lcdMainOnBottom();
	showingInventory = 0;
	setMiningDisabled(false);
	backButton.setVisible(false);
	saveButton.setVisible(false);
	craftButton.setVisible(false);
	pageButton.setVisible(false);
	clearText();
	drawBackground();
	drawInvButtons(false, isSurvival());
	updatePageName();
}

void moveSlotRecursive(bool right, int origSlot, bool first)
{
	bool *startOnBlock = new bool(getBlockID(mainPlayerInv.hand) != AIR);
	if (mainPlayerInv.hand == -1 || (mainPlayerInv.hand<-1 && getOpenedChestID() == -1))
		mainPlayerInv.hand = right ? (getOpenedChestID() == -1 ? 0 : -2) : (NUM_INV_SPACES - 1);
	else if (mainPlayerInv.hand<-1)
	{
		mainPlayerInv.hand += right ? -1 : 1;
		if (mainPlayerInv.hand>-2)
			mainPlayerInv.hand = -1;
		else if (mainPlayerInv.hand < -CHEST_SLOTS - 1)
			mainPlayerInv.hand = 0;
	}
	else
	{
		mainPlayerInv.hand += right ? 1 : -1;
		if (mainPlayerInv.hand >= NUM_INV_SPACES) mainPlayerInv.hand = -1;
		else if (mainPlayerInv.hand < 0) mainPlayerInv.hand = getOpenedChestID() == -1 ? (-1) : (-CHEST_SLOTS - 1);
	}
	if (getBlockID(mainPlayerInv.hand) == AIR&&!*startOnBlock && (mainPlayerInv.hand != origSlot || first))
	{
		delete startOnBlock;
		moveSlotRecursive(right, origSlot, false);
	}
}

void moveSelectedSlot(bool right)
{
	moveSlotRecursive(right, mainPlayerInv.hand, true);
	changeInvSelectedGraphic();
}

void updateInventory(touchPosition touch, WorldObject* world)
{
	if (showingInventory != 2) //Not in crafting menu
	{
		showGraphic(&selectedGraphic, 8, 6 * 8 + (getOpenedChestID() == -1 ? 0 : 4), false, 0);
		if (keysDown() & getGlobalSettings()->getKey(ACTION_ITEM_LEFT))
		{
			if (!isSurvival() && keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH))
				changeBlockPage(false);
			else
				moveSelectedSlot(false);
		}
		else if (keysDown() & getGlobalSettings()->getKey(ACTION_ITEM_RIGHT))
		{
			if (!isSurvival() && keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH))
				changeBlockPage(true);
			else
				moveSelectedSlot(true);
		}
	}

	switch (showingInventory)
	{
	case 0:
		if (keysDown() & getGlobalSettings()->getKey(ACTION_SWITCH_SCREEN))
			openInventory();
		break;
	case 1:
		if (keysDown() & KEY_TOUCH) //New Press
		{
			touchRead(&touch);
			backButton.setColored(backButton.isTouching(touch.px, touch.py));
			saveButton.setColored(saveButton.isTouching(touch.px, touch.py));
			craftButton.setColored(craftButton.isTouching(touch.px, touch.py));
			pageButton.setColored(pageButton.isTouching(touch.px, touch.py));
		}
		else if (keysUp() & KEY_TOUCH) //Release
		{
			if (touch.px > 1 * 8 && touch.py > (8 + 1)*8 && touch.py < (8 + 6)*8 && touch.px < 31 * 8)
			{
				int space = 0;
				space += 15 * ((touch.py - (8 + 1)*8) / (3 * 8));
				space += (touch.px - 8) / 16;
				if (getOpenedChestID() != -1 && mainPlayerInv.hand<-1 && world->chests[getOpenedChestID()].blocks[-mainPlayerInv.hand - 2].blockId != AIR)
				{ //Source: Chest, Destination: Inventory
					int tmpId, tmpAmount = 0;
					tmpId = world->chests[getOpenedChestID()].blocks[-mainPlayerInv.hand - 2].blockId;
					tmpAmount = world->chests[getOpenedChestID()].blocks[-mainPlayerInv.hand - 2].blockAmount;
					world->chests[getOpenedChestID()].blocks[-mainPlayerInv.hand - 2].blockId = mainPlayerInv.blocks[space].blockId;
					world->chests[getOpenedChestID()].blocks[-mainPlayerInv.hand - 2].blockAmount = mainPlayerInv.blocks[space].blockAmount;
					mainPlayerInv.blocks[space].blockId = tmpId;
					mainPlayerInv.blocks[space].blockAmount = tmpAmount;
					mainPlayerInv.hand = -1;
					setHand(-1);
				}
				else if (mainPlayerInv.hand > -1 && !(mainPlayerInv.blocks[mainPlayerInv.hand].blockId == AIR))
				{ //Source: Inventory, Destination:Inventory
					int tmpId, tmpAmount = 0;
					tmpId = mainPlayerInv.blocks[mainPlayerInv.hand].blockId;
					tmpAmount = mainPlayerInv.blocks[mainPlayerInv.hand].blockAmount;
					mainPlayerInv.blocks[mainPlayerInv.hand].blockId = mainPlayerInv.blocks[space].blockId;
					mainPlayerInv.blocks[mainPlayerInv.hand].blockAmount = mainPlayerInv.blocks[space].blockAmount;
					mainPlayerInv.blocks[space].blockId = tmpId;
					mainPlayerInv.blocks[space].blockAmount = tmpAmount;
					mainPlayerInv.hand = -1;
					setHand(-1);
				}
				else if (keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH) && getOpenedChestID() != -1)
				{
					int chestID = getOpenedChestID();
					for (int i = 0; i < CHEST_SLOTS; ++i)
						if (world->chests[chestID].blocks[i].blockId == AIR || world->chests[chestID].blocks[i].blockAmount == AIR)
						{
							int tmpId, tmpAmount = 0;
							tmpId = mainPlayerInv.blocks[space].blockId;
							tmpAmount = mainPlayerInv.blocks[space].blockAmount;
							mainPlayerInv.blocks[space].blockId = world->chests[getOpenedChestID()].blocks[i].blockId;
							mainPlayerInv.blocks[space].blockAmount = world->chests[getOpenedChestID()].blocks[i].blockAmount;
							world->chests[getOpenedChestID()].blocks[i].blockId = tmpId;
							world->chests[getOpenedChestID()].blocks[i].blockAmount = tmpAmount;
							mainPlayerInv.hand = -1;
							setHand(-1);
							break;
						}
				}
				else
				{
					mainPlayerInv.hand = space;
					setHand(space);
					changeInvSelectedGraphic(mainPlayerInv.blocks[space].blockId);
				}
			}
			else if (getOpenedChestID() != -1 && touch.px > 1 * 8 && touch.py > 1 * 8 && touch.py < 6 * 8 && touch.px < 31 * 8)
			{
				int space = 0;
				space += 15 * ((touch.py - 1 * 8) / (3 * 8));
				space += (touch.px - 8) / 16;
				if (mainPlayerInv.hand > -1 && mainPlayerInv.blocks[mainPlayerInv.hand].blockId != AIR)
				{ //Source: Inventory, Destination: Chest
					int tmpId, tmpAmount = 0;
					tmpId = mainPlayerInv.blocks[mainPlayerInv.hand].blockId;
					tmpAmount = mainPlayerInv.blocks[mainPlayerInv.hand].blockAmount;
					mainPlayerInv.blocks[mainPlayerInv.hand].blockId = world->chests[getOpenedChestID()].blocks[space].blockId;
					mainPlayerInv.blocks[mainPlayerInv.hand].blockAmount = world->chests[getOpenedChestID()].blocks[space].blockAmount;
					world->chests[getOpenedChestID()].blocks[space].blockId = tmpId;
					world->chests[getOpenedChestID()].blocks[space].blockAmount = tmpAmount;
					mainPlayerInv.hand = -1;
					setHand(getInventorySlot(-1));
				}
				else if (mainPlayerInv.hand < -1 && world->chests[getOpenedChestID()].blocks[-mainPlayerInv.hand - 2].blockId != AIR)
				{ //Source: Chest, Destination: Chest
					int tmpId, tmpAmount = 0;
					tmpId = world->chests[getOpenedChestID()].blocks[-mainPlayerInv.hand - 2].blockId;
					tmpAmount = world->chests[getOpenedChestID()].blocks[-mainPlayerInv.hand - 2].blockAmount;
					world->chests[getOpenedChestID()].blocks[-mainPlayerInv.hand - 2].blockId = world->chests[getOpenedChestID()].blocks[space].blockId;
					world->chests[getOpenedChestID()].blocks[-mainPlayerInv.hand - 2].blockAmount = world->chests[getOpenedChestID()].blocks[space].blockAmount;
					world->chests[getOpenedChestID()].blocks[space].blockId = tmpId;
					world->chests[getOpenedChestID()].blocks[space].blockAmount = tmpAmount;
					mainPlayerInv.hand = -1;
					setHand(-1);
				}
				else if (keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH))
				{
					addInventory(world->chests[getOpenedChestID()].blocks[space].blockId, world->chests[getOpenedChestID()].blocks[space].blockAmount, true);
					world->chests[getOpenedChestID()].blocks[space].blockId = AIR;
					world->chests[getOpenedChestID()].blocks[space].blockAmount = 0;
					mainPlayerInv.hand = -1;
					setHand(-1);
				}
				else
				{
					mainPlayerInv.hand = -space - 2;
					setHand(-space - 2);
					changeInvSelectedGraphic(world->chests[getOpenedChestID()].blocks[-mainPlayerInv.hand - 2].blockId);
				}
			}
			else if (backButton.isColored && backButton.isTouching(touch.px, touch.py))//(touch. px > (2 - 1)*8 && touch.px < (2 + 5)*8 && touch.py > (17 - 1)*8 && touch.py < (17 + 2)*8)
				closeInventory();
			else if (craftButton.isColored && craftButton.isTouching(touch.px, touch.py))
			{ //Crafting Menu
				backButton.setVisible(false);
				saveButton.setVisible(false);
				craftButton.setVisible(false);
				pageButton.setVisible(false);
				setInterface(INTERFACE_CRAFTING);
				showingInventory = 2;
			}
			else if (saveButton.isColored && saveButton.isTouching(touch.px, touch.py))
			{ //Save Game
				if (saveWorld(world))
					printLocalMessage("Saved Game\n");
				else
					printLocalMessage("Failed to Save Game\n");
				saveButton.setColored(false);
			}
			else if (pageButton.isColored && pageButton.isTouching(touch.px, touch.py))
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
				mainPlayerInv.hand = -1;
				setHand(-1);
				backButton.setColored(false);
				saveButton.setColored(false);
				craftButton.setColored(false);
				pageButton.setColored(false);
			}
		}
		if (keysDown() & getGlobalSettings()->getKey(ACTION_SWITCH_SCREEN))
			closeInventory();
		break;
	case 2:
		if (!isSurvival() && pageMenuUpdate(&touch))
			openInventory();
		break;
	}
}

void loadInventory(FILE* data)
{
	clearInventory();
	for (int i = 0; i < NUM_INV_SPACES; ++i)
	{
		int id, quantity;
		fscanf(data, "%d %d ", &id, &quantity);
		addInventory(id, quantity, true);
	}
}
