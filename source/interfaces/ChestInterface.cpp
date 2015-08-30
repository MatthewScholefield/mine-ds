#include "interfaceHandler.h"
#include "ChestInterface.h"
#include "../Config.h"
#include "../blockPages.h"
#include "../blockID.h"
#include "../graphics/inventoryGraphics.h"
#include "../blockName.h"
#include "../mining.h"
#include "../graphics/UI.h"
#include "../files.h"
#include "../chests.h"

void ChestInterface::updateInv()
{
	unloadGraphic(&selectedGraphic);
	loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, loadedGraphic = inv.hand < 0 ? AIR : inv.blocks[inv.hand].blockId);
	invHandler.drawSlots(!selectedChest);
	chestHandler->drawSlots(selectedChest);
	if (isSurvival())
	{
		invHandler.drawQuantities();
		chestHandler->drawQuantities();
	}
	updateTopName(inv.hand < 0 ? AIR : inv.blocks[inv.hand].blockId);
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
	return selectedChest ? *chest : inv;
}

void ChestInterface::moveSlot(bool right)
{
	Inventory *currentInv = &getSelectedInv();
	int change = right ? 1 : -1;
	int initialSlot = currentInv->hand;
	bool initialChest = selectedChest;

	if (currentInv->hand != -1 && currentInv->blocks[currentInv->hand].blockId != AIR)
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
		while ((currentInv->hand == -1 || currentInv->blocks[currentInv->hand].blockId == AIR)
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

void ChestInterface::closeInventory()
{
	if (selectedChest || inv.hand == -1)
		inv.hand = oldInvSlot;
	lcdMainOnBottom();
	setMiningDisabled(false);
	setInterface(INTERFACE_INVENTORY);
}

static bool touchesTileBox(const touchPosition &touch, int x, int y, int sx, int sy)
{
	return touch.px >= x * 8 && touch.px < x * 8 + sx * 8 && touch.py >= y * 8 && touch.py < y * 8 + sy * 8;
}

bool ChestInterface::touchesInvSlot(const touchPosition &touch)
{
	return (touchesTileBox(touch, 1, 9, 15 * 2, 1 * 2) || touchesTileBox(touch, 1, 12, 15 * 2, 1 * 2));
}

int ChestInterface::touchedSlot(const touchPosition& touch)
{
	const int OFF_X = 8;
	const int OFF_Y = 9 * 8;
	int row = (touch.px - OFF_X) / 16; //0..14
	int column = (touch.py - OFF_Y) / (3 * 8); //0..1
	return row + column * 15;
}

void ChestInterface::parseTouchInput(const touchPosition &touch)
{
	if (!(keysDown() & KEY_TOUCH) || !touchesInvSlot(touch))
		return;

	int touched = touchedSlot(touch);

	if (inv.hand < 0)
		inv.hand = touched;
	else
	{
		int tmpId = inv.blocks[inv.hand].blockId;
		int tmpAmount = inv.blocks[inv.hand].blockAmount;
		inv.blocks[inv.hand].blockId = inv.blocks[touched].blockId;
		inv.blocks[inv.hand].blockAmount = inv.blocks[touched].blockAmount;
		inv.blocks[touched].blockId = tmpId;
		inv.blocks[touched].blockAmount = tmpAmount;
		inv.hand = -1;
	}
	updateInv();
}

void ChestInterface::update(WorldObject *world, touchPosition *touch)
{
	if (!chest)
	{
		chestHandler = new InvGfxHandler(*(chest = &world->chests[getOpenedChestID()]), 1, 1);
		updateInv();
	}
	showGraphic(&selectedGraphic, 1 * 8, 6 * 8 + 4, false, 0);
	invHandler.update();
	chestHandler->update();

	parseKeyInput();
	parseTouchInput(*touch);
	switch (menu.update(*touch))
	{
	case BACK:
		closeInventory();
		break;
	case SAVE:
		if (saveWorld(world))
			printLocalMessage("Saved Game\n");
		else
			printLocalMessage("Failed to Save Game\n");
		break;
	case CRAFT_MENU:
		setInterface(INTERFACE_CRAFTING);
		break;
	case PAGE_MENU:
		break;
	default:
		break;
	}
	if (keysDown() & getGlobalSettings()->getKey(ACTION_SWITCH_SCREEN))
		closeInventory();
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