#include "FurnaceInterface.h"
#include "interfaceHandler.h"
#include "../graphics/UI.h"

void FurnaceInterface::update(WorldObject *world, touchPosition *touch)
{
	switch (menu.update(*touch))
	{
	case SMELT:
		break;
	case BACK:
		lcdMainOnBottom();
		setMiningDisabled(false);
		setInterface(INTERFACE_INVENTORY);
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
	drawBox(1, 8, 30, 8);
	printXY(6, 9, "Source");
	printXY(8, 14, "Fuel");
	drawInvSlot(12, 9);
	drawInvSlot(12, 13);
	drawInvSlot(17, 11);
}