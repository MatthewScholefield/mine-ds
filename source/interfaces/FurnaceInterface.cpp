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
}