#include "PageInterface.h"
#include "../../blockPages.h"
#include "interfaceHandler.h"
#include "../UI.h"

PageInterface::PageInterface() : Interface(INTERFACE_PAGE), menu(MENU_BUTTON, false)
{
	menu.addButton(0, 10, "\x011");
	menu.addButton(29, 10, "\x010");
	menu.addButton(1, 16, "Back");
	clearText();
	drawBackground();
	printXY(13, 11, getPageName());
}

void PageInterface::update(WorldObject *world, touchPosition *touch)
{
	switch (menu.update(*touch))
	{
	case LEFT:
		changeBlockPage(false, true);
		iprintf("\x1b[11;13H            ");
		printXY(13, 11, getPageName());
		break;
	case RIGHT:
		changeBlockPage(true, true);
		iprintf("\x1b[11;13H            ");
		printXY(13, 11, getPageName());
		break;
	case BACK:
		setInterface(INTERFACE_INVENTORY, true);
		break;
	default:
		break;
	}
}

void PageInterface::draw()
{
	menu.draw();
	iprintf("\x1b[11;13H            ");
	printXY(13, 11, getPageName());
}
