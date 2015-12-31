#include "FurnaceInterface.h"
#include "interfaceHandler.h"
#include "../UI.h"
#include "InventoryInterface.h"
#include "../../blocks.h"

void FurnaceInterface::updateContents()
{
	if (invOpen)
		return;
	if (openFurnace.inUse)
	{
		gfx[SOURCE].reload(GraphicType::BLOCK, openFurnace.sourceBlock.ID, false);
		gfx[FUEL].reload(GraphicType::BLOCK, openFurnace.fuelBlock.ID, false);
		gfx[RESULT].reload(GraphicType::BLOCK, openFurnace.resultBlock.ID, false);
		if (openFurnace.sourceBlock.amount > 0)
			printXY(12, 10, openFurnace.sourceBlock.amount);
		else
			printXY(12, 10, "  ");
		if (openFurnace.fuelBlock.amount > 0)
			printXY(12, 14, openFurnace.fuelBlock.amount);
		else
			printXY(12, 14, "  ");
		if (openFurnace.resultBlock.amount > 0)
			printXY(17, 12, openFurnace.resultBlock.amount);
		else
			printXY(17, 12, "  ");
	}
}

void FurnaceInterface::openInv()
{
	for (int i = 0; i < 3; ++i)
		gfx[i].reload(GraphicType::BLOCK, AIR, false);
	smeltButton->setVisible(false);
	invOpen = true;
	menu.setFrame(0, -1);
	draw();
}

void FurnaceInterface::closeInv()
{
	smeltButton->setVisible(true);
	selectedInvSlot = NONE;
	invOpen = false;
	menu.setFrame(0, 0);
	draw();
}

void FurnaceInterface::swapItem(InvBlock &original)
{
	InvBlock temp;
	switch (selectedInvSlot)
	{
	case FUEL:
		temp = openFurnace.fuelBlock;
		openFurnace.fuelBlock = original;
		break;
	case SOURCE:
		temp = openFurnace.sourceBlock;
		openFurnace.sourceBlock = original;
		break;
	default:
		showError("Invalid selected furnace slot");
		break;
	}
	original = temp;
}

void FurnaceInterface::update(World &world, touchPosition &touch)
{
	if (invOpen)
	{
		gfxHandler.update();
		if (keysDown() & KEY_TOUCH && InventoryInterface::touchesInvSlot(touch))
		{
			int slot = InventoryInterface::touchedSlot(touch);
			swapItem(getInventoryRef().blocks[slot]);
			closeInv();
		}
	}
	else
	{
		if (openFurnace.timeTillFuelBurn == 0)
			updateContents();
		gfx[SOURCE].draw(12 * 8, 9 * 8, false, 2);
		gfx[FUEL].draw(12 * 8, 13 * 8, false, 2);
		gfx[RESULT].draw(17 * 8, 11 * 8, false, 2);
		if (keysDown() & KEY_TOUCH)
		{
			if (touchesTileBox(touch, 12, 9, 2, 2))
				selectedInvSlot = SOURCE;
			else if (touchesTileBox(touch, 12, 13, 2, 2))
				selectedInvSlot = FUEL;
			else if (touchesTileBox(touch, 17, 11, 2, 2))
			{
				addInventory(openFurnace.resultBlock.ID, openFurnace.resultBlock.amount);
				openFurnace.resultBlock = InvBlock(AIR, 0);
				updateContents();
			}
			if (selectedInvSlot != NONE)
				openInv();
		}
	}
	switch (menu.update(touch))
	{
	case SMELT:
	{
		convertItemToFuel(openFurnace);
		openFurnace.fuelTillComplete = fuelNeeded(openFurnace);
		updateContents();
		break;
	}
	case BACK:
		if (invOpen)
			closeInv();
		else
		{
			lcdMainOnBottom();
			setMiningDisabled(false);
			setInterface(world, INTERFACE_INVENTORY);
		}
		break;
	default:
		break;
	}
}

void FurnaceInterface::draw()
{
	consoleClear();
	drawBackground();
	menu.draw();
	if (invOpen)
	{
		drawBoxFrame(0, 8, 32, 7);
		drawBoxCenter(1, 11, 30, 1);
		gfxHandler.drawSlots(-1);
		if (isSurvival())
			gfxHandler.drawQuantities();
	}
	else
	{
		drawBox(1, 8, 30, 8);
		printXY(6, 9, "Source");
		printXY(8, 14, "Fuel");
		drawInvSlot(12, 9);
		drawInvSlot(12, 13);
		drawInvSlot(17, 11);
		updateContents();
	}
}