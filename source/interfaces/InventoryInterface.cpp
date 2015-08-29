#include "interfaceHandler.h"
#include "InventoryInterface.h"
#include "../Config.h"
#include "../blockPages.h"
#include "../blockID.h"
#include "../graphics/inventoryGraphics.h"
#include "../blockName.h"
#include "../mining.h"
#include "../graphics/UI.h"
#include "../files.h"

void InventoryInterface::updateInv()
{
	unloadGraphic(&selectedGraphic);
	loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, loadedGraphic = inv.hand < 0 ? AIR : inv.blocks[inv.hand].blockId);
	gfxHandler.drawSlots(inv.hand);
	if (isSurvival())
		gfxHandler.drawQuantities();
	updateTopName(inv.hand < 0 ? AIR : inv.blocks[inv.hand].blockId);
}

void InventoryInterface::checkLimits(int &value)
{
	if (value > NUM_INV_SPACES - 1)
		value = -1;
	else if (value<-1)
		value = NUM_INV_SPACES - 1;
}

void InventoryInterface::moveSlot(bool right)
{
	int change = right ? 1 : -1;
	int invSlot = inv.hand;
	int initialSlot = invSlot;

	if (inv.blocks[invSlot].blockId != AIR)
	{
		invSlot += change;
		checkLimits(invSlot);
	}
	else
	{
		do
			checkLimits(invSlot += change);
		while (inv.blocks[invSlot].blockId == AIR && initialSlot != invSlot);
		if (initialSlot == invSlot)
		{
			invSlot += change;
			checkLimits(invSlot);
		}
	}
	inv.hand = invSlot;
	updateInv();
}

void InventoryInterface::parseKeyInput()
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

void InventoryInterface::openInventory()
{
	oldInvSlot = inv.hand;
	inv.hand = -1;
	lcdMainOnTop();
	setMiningDisabled(true);
	backButton->setVisible(true);
	backButton->draw();
	open = true;
	updateInv();
}

void InventoryInterface::closeInventory()
{
	if (inv.hand == -1)
		inv.hand = oldInvSlot;
	lcdMainOnBottom();
	setMiningDisabled(false);
	backButton->setVisible(false);
	open = false;
	draw();
}

static bool touchesTileBox(const touchPosition &touch, int x, int y, int sx, int sy)
{
	return touch.px >= x * 8 && touch.px < x * 8 + sx * 8 && touch.py >= y * 8 && touch.py < y * 8 + sy * 8;
}

bool InventoryInterface::touchesInvSlot(const touchPosition &touch)
{
	return (touchesTileBox(touch, 1, 9, 15 * 2, 1 * 2) || touchesTileBox(touch, 1, 12, 15 * 2, 1 * 2));
}

int InventoryInterface::touchedSlot(const touchPosition& touch)
{
	const int OFF_X = 8;
	const int OFF_Y = 9 * 8;
	int row = (touch.px - OFF_X) / 16; //0..14
	int column = (touch.py - OFF_Y) / (3 * 8); //0..1
	return row + column * 15;
}

void InventoryInterface::parseTouchInput(const touchPosition &touch)
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

void InventoryInterface::switchInvState()
{
	if (open)
		closeInventory();
	else
		openInventory();
}

void InventoryInterface::update(WorldObject *world, touchPosition *touch)
{
	showGraphic(&selectedGraphic, 1 * 8, 6 * 8, false, 0);
	//drawInvGraphics();
	gfxHandler.update();

	parseKeyInput();

	if (open)
	{
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
	}
	if (keysDown() & getGlobalSettings()->getKey(ACTION_SWITCH_SCREEN))
		switchInvState();
	if (shouldUpdate)
		updateInv();
}

void InventoryInterface::draw()
{
	consoleClear();
	drawBackground();
	menu.draw();
	drawBoxFrame(0, 8, 32, 7);
	drawBoxCenter(1, 11, 30, 1);
	drawSelectedFrame();
	updateInv();
}