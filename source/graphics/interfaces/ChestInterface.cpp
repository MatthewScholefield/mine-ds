#include "interfaceHandler.h"
#include "ChestInterface.h"
#include "../../Config.h"
#include "../../blockPages.h"
#include "../../blockID.h"
#include "../../blockName.h"
#include "../../mining.h"
#include "../UI.h"
#include "../../files.h"
#include "../../chests.h"

void ChestInterface::updateInv()
{
	Inventory curInv = getSelectedInv();
	unloadGraphic(&selectedGraphic);
	loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, loadedGraphic = curInv.hand < 0 ? AIR : curInv.blocks[curInv.hand].ID);
	invHandler.drawSlots(!selectedChest);
	chestHandler.drawSlots(selectedChest);
	if (isSurvival())
	{
		invHandler.drawQuantities();
		chestHandler.drawQuantities();
	}
	updateTopName(curInv.hand < 0 ? AIR : curInv.blocks[curInv.hand].ID);
}

int ChestInterface::correctValue(int value)
{
	if (value > NUM_INV_SPACES - 1)
	{
		selectedChest = !selectedChest;
		return -1;
	}
	else if (value < -1)
	{
		selectedChest = !selectedChest;
		return NUM_INV_SPACES - 1;
	}
	return value;
}

Inventory &ChestInterface::getSelectedInv()
{
	return selectedChest ? chest : inv;
}

void ChestInterface::moveSlot(bool right)
{
	Inventory *currentInv = &getSelectedInv();
	int change = right ? 1 : -1;
	int initialSlot = currentInv->hand;
	bool initialChest = selectedChest;

	if (currentInv->hand != -1 && currentInv->blocks[currentInv->hand].ID != AIR)
	{
		currentInv->hand += change;
		int val = correctValue(currentInv->hand);
		(currentInv = &getSelectedInv())->hand = val;
	}
	else
	{
		do
		{
			int val = correctValue(currentInv->hand += change);
			(currentInv = &getSelectedInv())->hand = val;
		}
		while ((currentInv->hand == -1 || currentInv->blocks[currentInv->hand].ID == AIR)
				&& (initialSlot != currentInv->hand || initialChest != selectedChest));
		if (initialSlot == currentInv->hand && initialChest == selectedChest)
		{
			currentInv->hand += change;
			getSelectedInv().hand = correctValue(currentInv->hand);
		}
	}
	updateInv();
}

void ChestInterface::parseKeyInput()
{
	if (keysDown() & getGlobalSettings()->getKey(ACTION_ITEM_LEFT))
	{
		if (!isSurvival() && keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH))
			changeBlockPage(false);
		else
			moveSlot(false);
	}
	else if (keysDown() & getGlobalSettings()->getKey(ACTION_ITEM_RIGHT))
	{
		if (!isSurvival() && keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH))
			changeBlockPage(true);
		else
			moveSlot(true);
	}
}

void ChestInterface::closeInventory(WorldObject &world)
{
	if (selectedChest || getSelectedInv().hand == -1)
		inv.hand = oldInvSlot;
	lcdMainOnBottom();
	setMiningDisabled(false);
	setInterface(world, INTERFACE_INVENTORY);;
}

bool ChestInterface::touchesInvSlot(const touchPosition &touch)
{
	return (touchesTileBox(touch, 1, 9, 15 * 2, 1 * 2) || touchesTileBox(touch, 1, 12, 15 * 2, 1 * 2));
}

bool ChestInterface::touchesChestSlot(const touchPosition &touch)
{
	return (touchesTileBox(touch, 1, 1, 15 * 2, 1 * 2) || touchesTileBox(touch, 1, 4, 15 * 2, 1 * 2));
}

int ChestInterface::touchedSlot(const touchPosition& touch, int yOffset)
{
	const int OFF_X = 8;
	int row = (touch.px - OFF_X) / 16; //0..14
	int column = (touch.py - yOffset) / (3 * 8); //0..1
	return row + column * 15;
}

void ChestInterface::parseTouchInput(const touchPosition &touch)
{
	if (!(keysDown() & KEY_TOUCH) || (!touchesInvSlot(touch)&&!touchesChestSlot(touch)))
		return;

	bool touchedChest = touchesChestSlot(touch);
	int touched = touchedSlot(touch, touchedChest ? 1 : 9 * 8);
	bool jumpTransfer = keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH);

	if (getSelectedInv().hand < 0 && !jumpTransfer)
	{
		selectedChest = touchedChest;
		getSelectedInv().hand = touched;
	}
	else
	{
		if (jumpTransfer)
		{
			selectedChest = !touchedChest;
			int i = 0;
			for (; i < NUM_INV_SPACES && getSelectedInv().blocks[i].ID != AIR; ++i);
			if (i == NUM_INV_SPACES)
			{
				getSelectedInv().hand = -1;
				updateInv();
				return;
			}
			getSelectedInv().hand = i;

		}
		Inventory &src = getSelectedInv();
		selectedChest = touchedChest;
		Inventory &dest = getSelectedInv();
		int tmpId = src.blocks[src.hand].ID;
		int tmpAmount = src.blocks[src.hand].amount;
		src.blocks[src.hand].ID = dest.blocks[touched].ID;
		src.blocks[src.hand].amount = dest.blocks[touched].amount;
		dest.blocks[touched].ID = tmpId;
		dest.blocks[touched].amount = tmpAmount;
		dest.hand = -1;
	}
	updateInv();
}

void ChestInterface::update(WorldObject &world, touchPosition &touch)
{
	showGraphic(&selectedGraphic, 1 * 8, 6 * 8 + 4, false, 0);
	invHandler.update();
	chestHandler.update();

	parseKeyInput();
	parseTouchInput(touch);
	switch (menu.update(touch))
	{
	case BACK:
		closeInventory(world);
		break;
	case SAVE:
		if (saveWorld(world))
			printLocalMessage("Saved Game\n");
		else
			printLocalMessage("Failed to Save Game\n");
		break;
	case CRAFT_MENU:
		setInterface(world, INTERFACE_CRAFTING);;
		break;
	case PAGE_MENU:
		setInterface(world, INTERFACE_PAGE);;
		break;
	default:
		break;
	}
	if (keysDown() & getGlobalSettings()->getKey(ACTION_SWITCH_SCREEN))
		closeInventory(world);
	if (shouldUpdate)
		updateInv();
}

void ChestInterface::drawHandFrame()
{
	setSubBgTile(0, 6, 27);
	setSubBgTile(0, 7, 27);
	setSubBgTile(0, 8, 27);
	setSubBgTile(3, 6, 28);
	setSubBgTile(3, 7, 27, H_FLIP);
	setSubBgTile(3, 8, 28);
	drawBoxCenter(1, 6, 2, 3);
}

void ChestInterface::draw()
{
	consoleClear();
	drawBackground();
	menu.draw();
	drawBoxFrame(0, 8, 32, 7);
	drawBoxCenter(1, 11, 30, 1);

	drawBoxFrame(0, 0, 32, 7);
	drawBoxCenter(1, 3, 30, 1);

	drawHandFrame();
	updateInv();
}