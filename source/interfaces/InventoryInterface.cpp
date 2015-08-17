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

bool InventoryInterface::shouldUpdate = false;

void InventoryInterface::triggerUpdate()
{
	shouldUpdate = true;
}

void InventoryInterface::staticUpdate()
{
	shouldUpdate = false;
}

void InventoryInterface::updateInv()
{
	unloadGraphic(&selectedGraphic);
	loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, loadedGraphic = invSlot < 0 ? AIR : getBlockID(invSlot));
	drawSlots(invSlot, 1, 9);
	drawQuantity(false, 1, 10, 15, 2, 2, 3);
	updateTopName(getBlockID(invSlot));
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

	if (getBlockID(invSlot) != AIR)
	{
		invSlot += change;
		checkLimits(invSlot);
	}
	else
	{
		do
			checkLimits(invSlot += change);
		while (getBlockID(invSlot) == AIR && initialSlot != invSlot);
		if (initialSlot == invSlot)
		{
			invSlot += change;
			checkLimits(invSlot);
		}
	}
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
	oldInvSlot = invSlot;
	invSlot = -1;
	lcdMainOnTop();
	setMiningDisabled(true);
	backButton->setVisible(true);
	backButton->draw();
	open = true;
}

void InventoryInterface::closeInventory()
{
	if (invSlot == -1)
		invSlot = oldInvSlot;
	lcdMainOnBottom();
	setMiningDisabled(false);
	backButton->setVisible(false);
	open = false;
	draw();
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