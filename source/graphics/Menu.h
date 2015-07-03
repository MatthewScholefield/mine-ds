#pragma once
#include "Button.h"
#include <memory>
#include <vector>
#include <string>

void slideButtonAction(UIElement *button, int sizeY, bool extra);
typedef std::shared_ptr<UIElement> UIElement_ptr;

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

	std::vector<UIElement_ptr> elements;
	std::vector<std::string> listItems;

public:
	void draw(bool labels = true);
	int activate(bool init = true);
	void setListXY(int x, int y);
	void setFrame(int x, int y = 0);
	void setAction(void (*function)(UIElement *element, int data, bool data2), int sendData);

	void addButton(int x, int y, const char * const label, int length = -1, bool isVisible = true, bool checkButton = false, bool initial = false, CheckSet checkSet = CHECK_SET_NONE);
	void addButton(int x, int y, const char * const label, bool isVisible, bool checkButton = false, bool initial = false, CheckSet checkSet = CHECK_SET_NONE);

	void addListItem(const char *label);

	Menu(MenuType type = MENU_BUTTON, bool back = true, int sizeY = 24, bool initial = false) : type(type), frameX(0), frameY(0), sizeX(32), sizeY(sizeY) {
		elements.push_back(UIElement_ptr(new Button(25, 20, "Back", back)));
		elements.push_back(UIElement_ptr(new Button(1, 20, "\x1F", sizeY > 24)));
		setAction(slideButtonAction, sizeY);
		switch (type) {
			case MENU_BOOL:
				elements.push_back(UIElement_ptr(new Button(9, 10, "Enabled", 12, true, true, initial, CHECK_SET_NONE)));
				elements.push_back(UIElement_ptr(new Button(9, 15, "Disabled", 12, true, true, !initial, CHECK_SET_NONE)));
				break;
			default:
				break;
		}
	}
};