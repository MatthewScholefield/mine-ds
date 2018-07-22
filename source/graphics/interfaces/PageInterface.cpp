#include "PageInterface.h"
#include "../../blockPages.h"
#include "interfaceHandler.h"
#include "../UI.h"

#include "../../localizations/locale.h"

PageInterface::PageInterface() : Interface(INTERFACE_PAGE), menu(MENU_BUTTON, false)
{
	menu.addButton(0, 10, "\x011");
	menu.addButton(29, 10, "\x010");
	menu.addButton(1, 16, locale("back"));
	clearText();
	drawBackground();
	printXY(13, 11, getPageName());
}

void PageInterface::update(World &world, touchPosition &touch)
{
	switch (menu.update(touch))
	{
	case LEFT:
		changeBlockPage(false, true);
		printf("\x1b[11;13H            ");
		printXY(13, 11, getPageName());
		break;
	case RIGHT:
		changeBlockPage(true, true);
		printf("\x1b[11;13H            ");
		printXY(13, 11, getPageName());
		break;
	case BACK:
		setInterface(world, INTERFACE_INVENTORY, true);
		break;
	default:
		break;
	}
}

void PageInterface::draw()
{
	menu.draw();
	printf("\x1b[11;13H            ");
	printXY(13, 11, getPageName());
}
