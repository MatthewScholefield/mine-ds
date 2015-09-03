#include "FurnaceInterface.h"
#include "interfaceHandler.h"
#include "../graphics/UI.h"
#include "InventoryInterface.h"

void FurnaceInterface::updateContents()
{
	for (int i = 0; i < 3; ++i)
		unloadGraphic(&gfx[i]);
	loadGraphicSub(&gfx[SOURCE], GRAPHIC_BLOCK, openFurnace->source.blockId);
	loadGraphicSub(&gfx[FUEL], GRAPHIC_BLOCK, openFurnace->fuel.blockId);
	loadGraphicSub(&gfx[RESULT], GRAPHIC_BLOCK, openFurnace->result.blockId);
	printXY(12, 10, openFurnace->source.blockAmount);
	printXY(12, 14, openFurnace->fuel.blockAmount);
	printXY(17, 12, openFurnace->result.blockAmount);
}

void FurnaceInterface::openInv()
{
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
		temp = openFurnace->fuel;
		openFurnace->fuel = original;
		break;
	case SOURCE:
		temp = openFurnace->source;
		openFurnace->source = original;
		break;
	default:
		showError("Invalid selected furnace slot");
		break;
	}
	original = temp;
}

void FurnaceInterface::update(WorldObject *world, touchPosition *touch)
{
	if (!openFurnace)
		openFurnace = world->furnaces[getOpenedFurnaceID()];
	showGraphic(&gfx[SOURCE], 12 * 8, 9 * 8);
	showGraphic(&gfx[FUEL], 12 * 8, 13 * 8);
	showGraphic(&gfx[RESULT], 17 * 8, 11 * 8);
	loadGraphicSub(&gfx[FUEL], GRAPHIC_BLOCK, openFurnace->fuel.blockId);
	loadGraphicSub(&gfx[RESULT], GRAPHIC_BLOCK, openFurnace->result.blockId);
	printXY(12, 9, openFurnace->source.blockAmount);
	printXY(12, 13, openFurnace->fuel.blockAmount);
	printXY(17, 11, openFurnace->result.blockAmount);
	if (invOpen)
	{
		gfxHandler.update();
		if (InventoryInterface::touchesInvSlot(*touch))
		{
			int slot = InventoryInterface::touchedSlot(*touch);
			swapItem(getInventoryRef().blocks[slot]);
			closeInv();
		}
	}
	else
	{
		if (touchesTileBox(*touch, 12, 9, 2, 2))
			selectedInvSlot = FUEL;
		else if (touchesTileBox(*touch, 12, 13, 2, 2))
			selectedInvSlot = SOURCE;
		if (selectedInvSlot != NONE)
			openInv();
	}
	switch (menu.update(*touch))
	{
	case SMELT:
		break;
	case BACK:
		if (invOpen)
			closeInv();
		else
		{
			lcdMainOnBottom();
			setMiningDisabled(false);
			setInterface(INTERFACE_INVENTORY);
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
	}
}