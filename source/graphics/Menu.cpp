#include "Button.h"
#include "Menu.h"
#include "graphics.h"
#include "UI.h"
#include <vector>
#include <string>
#include <nds.h>

void Menu::draw(bool labels)
{
	switch (type)
	{
		case MENU_BOOL:
		case MENU_BUTTON:
			for (std::vector<Button>::size_type i = 0; i != buttons.size(); ++i)
				if (buttons[i].visible)
					buttons[i].draw(labels);
			break;
		case MENU_LIST:
			if (labels)
				for (std::vector<std::string>::size_type i = 0; i != listItems.size(); ++i)
					printXY(frameX + listX + 1, frameY + listY + 1 + i, listItems[i].c_str());

			drawBox(frameX + listX, frameY + listY, maxStringLength(listItems) + 2, listItems.size() + 2);
			break;
	}
}

void Menu::setListXY(int x, int y)
{
	listX = x;
	listY = y;
}

void Menu::setFrame(int x, int y)
{
	for (uint i = 0; i < buttons.size(); ++i)
	{
		buttons[i].x += x - frameX;
		buttons[i].printX += x - frameX;
		buttons[i].y += y - frameY;
		buttons[i].printY += y - frameY;
	}
	frameX = x;
	frameY = y;
}

void Menu::addButton(int x, int y, const char * const label, int length, bool isVisible)
{
	buttons.emplace_back(x + frameX, y + frameY, label, length, isVisible);
}

void Menu::addButton(int x, int y, const char * const label, bool isVisible)
{
	buttons.emplace_back(x + frameX, y + frameY, label, isVisible);
}

void Menu::addListItem(const char* label)
{
	listItems.emplace_back(label);
}

int Menu::activate(bool initial)
{
	swiWaitForVBlank();
	draw(true);
	switch (type)
	{
		case MENU_BOOL:
			buttons[2].isColored = initial;
			buttons[3].isColored = !initial;
		case MENU_BUTTON:
		{
			touchPosition touch;
			int selected = -1;
			bool chosen = false;

			while (!chosen)
			{
				updateSubBG();
				updateFrame();
				updateTime(); //Used to ensure random world seed changes
				scanKeys();
				if (keysDown() & KEY_TOUCH)
				{
					touchRead(&touch);
					for (uint i = 0; i < (type == MENU_BOOL ? 2 : buttons.size()); ++i)
						if (buttons[i].isTouching(touch.px, touch.py))
							buttons[i].setColored(true);
					if (type == MENU_BOOL)
					{
						for (int i = 2; i < 4; ++i)
							buttons[i].setColored(buttons[i].isTouching(touch.px, touch.py));
						selected = buttons[3].isColored;
					}
				}
				else if (keysHeld() & KEY_TOUCH)
					touchRead(&touch);
				else if (keysUp() & KEY_TOUCH)
				{
					for (uint i = 0; i < buttons.size(); ++i)
					{
						if (buttons[i].isColored && buttons[i].isTouching(touch.px, touch.py))
						{
							switch (i)
							{
								case 0: //Back
									if (type != MENU_BOOL)
										selected = 0;
									chosen = true;
									break;
								case 1: //Scroll
									if (getScrollY() / 8 < (sizeY - 24) / 2)
									{
										moveSubBg(0, (sizeY - 24)*8);
										buttons[1].label = "\x1E";
										for (uint i = 0; i < buttons.size(); ++i)
											if (buttons[i].printY >= 24)
												buttons[i].printLabel();
									}
									else
									{
										moveSubBg(0, -(sizeY - 24)*8);
										buttons[1].label = "\x1F";
									}
									buttons[1].printLabel();
									break;
								default:
									if (type == MENU_BOOL)
										break;
									selected = i - 1;
									chosen = true;
									break;
							}
						}
					}
					for (uint i = 0; i < (type == MENU_BOOL ? 2 : buttons.size()); ++i)
						buttons[i].setColored(false);
				}
			}
			moveSubBg(0, -64);
			return selected;
		}
		case MENU_LIST:
		{
			touchPosition touch;
			int maxNameLength = maxStringLength(listItems);
			scanKeys();
			touchRead(&touch);
			uint column = 0;
			while (1)
			{
				updateSubBG();
				scanKeys();
				if (keysDown() & KEY_TOUCH) //New Press
				{
					touchRead(&touch);
					column = ((touch.py - 8) / 8) + 1 - (frameY + listY);
					if (column <= listItems.size() && column >= 1 && (touch.px + getScrollX()) % 512 >= (frameX + listX + 1) * 8 && (touch.px + getScrollX()) % 512 < (frameX + listX + maxNameLength + 1) * 8)
						for (int i = 0; i < maxNameLength; ++i)
							setSubBgTile(frameX + listX + 1 + i, frameY + listY + column, 60);
					else if (buttons[0].isTouching(touch.px, touch.py))
						buttons[0].setColored(true);
				}
				else if (keysHeld() & KEY_TOUCH)
					touchRead(&touch);
				else if (keysUp() & KEY_TOUCH)
				{
					uint newColumn = ((touch.py - 8) / 8) + 1 - (frameY + listY);
					if (buttons[0].isColored && buttons[0].isTouching(touch.px, touch.py))
						return 0;
					else if (column == newColumn && column <= listItems.size() && column >= 1 && (touch.px + getScrollX()) % 512 >= (frameX + listX + 1) * 8 && (touch.px + getScrollX()) % 512 < (frameX + listX + maxNameLength + 1) * 8)
						return column;
					else //Remove any colored buttons, if any
					{
						drawBoxCenter(frameX + listX + 1, frameY + listY + 1, maxNameLength, listItems.size());
						buttons[0].setColored(false);
					}
				}
				updateFrame();
			}
		}
	}
	return 0;
}