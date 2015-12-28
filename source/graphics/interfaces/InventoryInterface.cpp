#include "interfaceHandler.h"
#include "InventoryInterface.h"
#include "../../Config.h"
#include "../../blockPages.h"
#include "../../blockID.h"
#include "../../blockName.h"
#include "../../mining.h"
#include "../UI.h"
#include "../../files.h"
#include "../../mobs/BaseMob.h"
#include "../../mobs/mobHandler.h"
#include "../../general.h"


void InventoryInterface::updateInv()
{
	selectedGraphic.reload(GraphicType::BLOCK, loadedGraphic = inv.hand < 0 ? AIR : inv.blocks[inv.hand].ID, false);
	gfxHandler.drawSlots(inv.hand);
	if (isSurvival())
		gfxHandler.drawQuantities();
	updateTopName(inv.hand < 0 ? AIR : inv.blocks[inv.hand].ID);
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

	if (invSlot != -1 && inv.blocks[invSlot].ID != AIR)
	{
		invSlot += change;
		checkLimits(invSlot);
	}
	else
	{
		do
			checkLimits(invSlot += change);
		while ((invSlot == -1 || inv.blocks[invSlot].ID == AIR) && initialSlot != invSlot);
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
		int tmpId = inv.blocks[inv.hand].ID;
		int tmpAmount = inv.blocks[inv.hand].amount;
		inv.blocks[inv.hand].ID = inv.blocks[touched].ID;
		inv.blocks[inv.hand].amount = inv.blocks[touched].amount;
		inv.blocks[touched].ID = tmpId;
		inv.blocks[touched].amount = tmpAmount;
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

void InventoryInterface::arrangeItems(bool right)
{
	Inventory newInv;
	newInv.hand = inv.hand;
	int position = right ? NUM_INV_SPACES - 1 : 0;
	const int INITIAL = position;
	const int ADD = right ? 1 : -1;
	for (int i = right ? 0 : (NUM_INV_SPACES - 1); i != INITIAL + ADD; i += ADD)
	{
		if (inv.blocks[i].ID == AIR)
			continue;
		InvBlock &block = inv.blocks[i];
		int j;
		for (j = INITIAL; j != position; j -= ADD)
			if (newInv.blocks[j].ID == block.ID && newInv.blocks[j].amount < 64)
			{
				newInv.blocks[j].amount += block.amount;
				if (newInv.blocks[j].amount > 64)
				{
					block.amount = newInv.blocks[j].amount - 64;
					newInv.blocks[j].amount = 64;
				}
				else
				{
					block.ID = AIR;
					break;
				}
			}
		if (j == position) //Didn't find a match
		{
			newInv.blocks[j] = block;
			position -= ADD;
		}
	}
	inv = newInv;
	updateInv();
}

void InventoryInterface::update(WorldObject &world, touchPosition &touch)
{
	if (inv.hand > 32) inv.hand = -1;
	if (oldInvSlot > 32) oldInvSlot = -1;
	selectedGraphic.draw(1 * 8, 6 * 8, false, 0);
	gfxHandler.update();

	parseKeyInput();
	if (open)
	{
		parseTouchInput(touch);
		switch (menu.update(touch))
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
			setInterface(world, INTERFACE_CRAFTING, false);
			break;
		case PAGE_MENU:
			setInterface(world, INTERFACE_PAGE);
			break;
		case ARRANGE_LEFT:
			arrangeItems(false);
			break;
		case ARRANGE_RIGHT:
			arrangeItems(true);
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

void InventoryInterface::drawHandFrame()
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

void InventoryInterface::draw()
{
	consoleClear();
	drawBackground();
	menu.draw();

	//Draw Inventory container
	drawBoxFrame(0, 8, 32, 7);
	drawBoxCenter(1, 11, 30, 1);

	drawHandFrame();
	updateInv();
}