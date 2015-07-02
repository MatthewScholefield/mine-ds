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
		case MENU_LIST:
			if (labels)
				for (std::vector<std::string>::size_type i = 0; i != listItems.size(); ++i)
					printXY(frameX + listX + 1, frameY + listY + 1 + i, listItems[i].c_str());

			drawBox(frameX + listX, frameY + listY, maxStringLength(listItems) + 2, listItems.size() + 2);
		case MENU_BOOL:
		case MENU_BUTTON:
			for (std::vector<Button>::size_type i = 0; i != elements.size(); ++i)
				if (elements[i].isVisible)
					elements[i].draw(labels);
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
	for (uint i = 0; i < elements.size(); ++i)
	{
		elements[i].x += x - frameX;
		elements[i].printX += x - frameX;
		elements[i].y += y - frameY;
		elements[i].printY += y - frameY;
	}
	frameX = x;
	frameY = y;
}

void Menu::addButton(int x, int y, const char * const label, int length, bool isVisible)
{
	elements.emplace_back(x + frameX, y + frameY, label, length, isVisible);
}

void Menu::addButton(int x, int y, const char * const label, bool isVisible)
{
	elements.emplace_back(x + frameX, y + frameY, label, isVisible);
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
			elements[2].setColored(initial);
			elements[3].setColored(!initial);
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
					for (uint i = 0; i < (type == MENU_BOOL ? 2 : elements.size()); ++i)
						if (elements[i].isTouching(touch.px, touch.py))
							elements[i].setColored(true);
					if (type == MENU_BOOL)
					{

						if (elements[2].isTouching(touch.px, touch.py))
							elements[2].setColored(true);
						else if (elements[3].isTouching(touch.px, touch.py))
							elements[2].setColored(false);
						elements[3].setColored(!elements[2].isColored);
						selected = elements[2].isColored;
					}
				}
				else if (keysHeld() & KEY_TOUCH)
					touchRead(&touch);
				else if (keysUp() & KEY_TOUCH)
				{
					for (uint i = 0; i < elements.size(); ++i)
					{
						if (elements[i].isColored && elements[i].isTouching(touch.px, touch.py))
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
										elements[1].label = "\x1E";
									}
									else
									{
										moveSubBg(0, -(sizeY - 24)*8);
										elements[1].label = "\x1F";
									}
									elements[1].draw();
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
					for (uint i = 0; i < (type == MENU_BOOL ? 2 : elements.size()); ++i)
						elements[i].setColored(false);
				}
			}
			moveSubBg(0, -64);
			if (type == MENU_BOOL)
				return elements[2].isColored;
			else
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
					else if (elements[0].isTouching(touch.px, touch.py))
						elements[0].setColored(true);
				}
				else if (keysHeld() & KEY_TOUCH)
					touchRead(&touch);
				else if (keysUp() & KEY_TOUCH)
				{
					uint newColumn = ((touch.py - 8) / 8) + 1 - (frameY + listY);
					if (elements[0].isColored && elements[0].isTouching(touch.px, touch.py))
						return 0;
					else if (column == newColumn && column <= listItems.size() && column >= 1 && (touch.px + getScrollX()) % 512 >= (frameX + listX + 1) * 8 && (touch.px + getScrollX()) % 512 < (frameX + listX + maxNameLength + 1) * 8)
						return column;
					else //Remove any colored buttons, if any
					{
						drawBoxCenter(frameX + listX + 1, frameY + listY + 1, maxNameLength, listItems.size());
						elements[0].setColored(false);
					}
				}
				updateFrame();
			}
		}
	}
	return 0;
}