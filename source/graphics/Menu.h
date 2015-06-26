#pragma once
#include "Button.h"
#include <vector>
#include <string>

enum MenuType {
	MENU_BUTTON,
	MENU_LIST,
	MENU_BOOL
};

class Menu {
private:
	MenuType type;
	int frameX, frameY; //In tiles
	int sizeX, sizeY; //In tiles
	int listX, listY; //For list menus
	
	std::vector<Button> buttons;
	std::vector<std::string> listItems;

public:
	void draw(bool labels = true);
	int activate(bool init = true);
	void setListXY(int x, int y);
	void setFrame(int x, int y = 0);

	void addButton(int x, int y, const char * const label, int length = -1, bool isVisible = true);
	void addButton(int x, int y, const char * const label, bool isVisible);

	void addListItem(const char *label);

	Menu(MenuType type = MENU_BUTTON, bool back = true, int sizeY = 24) : type(type), frameX(0), frameY(0), sizeX(32), sizeY(sizeY) {
		buttons.emplace_back(25, 20, "Back", back);
		buttons.emplace_back(1, 20, "\x1F", sizeY > 24);
		switch (type) {
			case MENU_BOOL:
				buttons.emplace_back(9, 10, "Enabled", 12);
				buttons.emplace_back(9, 15, "Disabled", 12);
				break;
			default:
				break;
		}
	}
};