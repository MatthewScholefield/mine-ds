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

void InventoryInterface::changeInvSelectedGraphic()
{
	unloadGraphic(&selectedGraphic);
	loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, loadedGraphic = invSlot < 0 ? AIR : getBlockID(invSlot));
	drawSlots(invSlot, 1, 9);
	updateTopName(getBlockID(invSlot));
}

void InventoryInterface::updateInvGraphics()
{
	drawBoxFrame(0, 8, 32, 7);
	drawBoxCenter(1, 11, 30, 1);
	drawSelectedFrame();
	drawQuantity(false, 1, 10, 15, 2, 2, 3);
	updateTopName(getBlockID(invSlot));
	changeInvSelectedGraphic();
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
	int initialSlot = invSlot;

	if (checkInventorySlot(invSlot) != AIR)
	{
		invSlot += change;
		checkLimits(invSlot);
	}
	else
	{
		do
			checkLimits(invSlot += change);
		while (checkInventorySlot(invSlot) == AIR && initialSlot != invSlot);
		if (initialSlot == invSlot)
		{
			invSlot += change;
			checkLimits(invSlot);
		}
	}
	updateInvGraphics();
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
	oldInvSlot = invSlot;
	invSlot = -1;
	updateInvGraphics();
	lcdMainOnTop();
	setMiningDisabled(true);
	open = true;
}

void InventoryInterface::closeInventory()
{
	if (invSlot == -1)
		invSlot = oldInvSlot;
	lcdMainOnBottom();
	setMiningDisabled(false);
	clearText();
	drawBackground();
}

void InventoryInterface::update(WorldObject *world, touchPosition *touch)
{
	showGraphic(&selectedGraphic, 1 * 8, 6 * 8, false, 0);
	drawInvGraphics();

	parseKeyInput();

	if (open)
	{
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
	else
	{
		if (keysDown() & getGlobalSettings()->getKey(ACTION_SWITCH_SCREEN))
			openInventory();
	}
}

void InventoryInterface::draw()
{
	menu.draw();
}