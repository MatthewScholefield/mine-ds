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

bool ChestInterface::shouldUpdate = false;

void ChestInterface::triggerUpdate()
{
	shouldUpdate = true;
}

void ChestInterface::staticUpdate()
{
	shouldUpdate = false;
}

void ChestInterface::updateInv()
{
	unloadGraphic(&selectedGraphic);
	loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, loadedGraphic = getHand() < 0 ? AIR : getBlockID(getHand()));
	drawSlots(getHand(), 1, 9);
	updateTopName(getBlockID(getHand()));

	if (isSurvival())
	{
		for (int i = 0; i < 15; ++i)
			for (int j = 0; j < 2; ++j)
				if (getBlockAmount(j * 15 + i) != 0 && getBlockID(j * 15 + i) != 0)
				{
					printXY(1 + i * 2, 10 + j * 3, getBlockAmount(j * 15 + i));
					if (getBlockAmount(j * 15 + i) < 10)
						printXY(1 + i * 2 + 1, 10 + j * 3, " ");
				}
				else
					printXY(1 + i * 2, 10 + j * 3, "  ");
	}
}

void ChestInterface::checkLimits(int &value)
{
	if (value > NUM_INV_SPACES - 1)
		value = -1;
	else if (value<-1)
		value = NUM_INV_SPACES - 1;
}

void ChestInterface::moveSlot(bool right)
{
	int change = right ? 1 : -1;
	int invSlot = getHand();
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
	setHand(invSlot);
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

void ChestInterface::openInventory()
{
	oldInvSlot = getHand();
	setHand(-1);
	lcdMainOnTop();
	setMiningDisabled(true);
	backButton->setVisible(true);
	backButton->draw();
	updateInv();
}

void ChestInterface::closeInventory()
{
	if (getHand() == -1)
		setHand(oldInvSlot);
	lcdMainOnBottom();
	setMiningDisabled(false);
	backButton->setVisible(false);
	draw();
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

	if (getHand() == -1)
		setHand(touched);
	else
	{
		int tmpId = getBlockID(getHand());
		int tmpAmount = getBlockAmount(getHand());
		setBlockID(getHand(), getBlockID(touched));
		setBlockAmount(getHand(), getBlockAmount(touched));
		setBlockID(touched, tmpId);
		setBlockAmount(touched, tmpAmount);
		setHand(-1);
	}
	updateInv();
}

void ChestInterface::update(WorldObject *world, touchPosition *touch)
{
	showGraphic(&selectedGraphic, 1 * 8, 6 * 8, false, 0);
	drawInvGraphics();

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

void ChestInterface::draw()
{
	consoleClear();
	drawBackground();
	menu.draw();
	drawBoxFrame(0, 8, 32, 7);
	drawBoxCenter(1, 11, 30, 1);
	drawSelectedFrame();
	updateInv();
}