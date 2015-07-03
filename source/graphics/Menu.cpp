#include "Button.h"
#include "Menu.h"
#include "graphics.h"
#include "UI.h"
#include "Slider.h"
#include "CheckButton.h"
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
		case MENU_BUTTON:
			for (std::vector<UIElement_ptr>::size_type i = 0; i != elements.size(); ++i)
				if (elements[i]->isVisible)
					elements[i]->draw(labels);
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
	for (std::vector<UIElement_ptr>::size_type i = 0; i != elements.size(); ++i)
		elements[i]->move(x - frameX, y - frameY);
	frameX = x;
	frameY = y;
}

void Menu::addButton(int x, int y, const char * const label, int length, bool isVisible)
{
	elements.push_back(UIElement_ptr(new Button(x + frameX, y + frameY, label, length, isVisible)));
}

void Menu::addButton(int x, int y, const char * const label, bool isVisible)
{
	elements.push_back(UIElement_ptr(new Button(x + frameX, y + frameY, label, isVisible)));
}

void Menu::addListItem(const char* label)
{
	listItems.emplace_back(label);
}

void Menu::addSlider(int x, int y, const char * const label, int initPos, int length, bool visible)
{
	elements.push_back(UIElement_ptr(new Slider(x + frameX, y + frameY, label, initPos, length, visible)));
}

void Menu::addCheckButton(int x, int y, const char* const label, bool enabled, int length, bool isVisible)
{
	elements.push_back(UIElement_ptr(new CheckButton(x + frameX, y + frameY, label, enabled, length, isVisible)));
}

void slideButtonAction(UIElement *button, int sizeY, bool extra)
{
	if (strcmp(button->label, "\x1E") == 0) //Slide up
	{
		moveSubBg(0, -(sizeY - 24)*8);
		button->label = "\x1F";
	}
	else //Slide down
	{
		moveSubBg(0, (sizeY - 24)*8);
		button->label = "\x1E";
	}
	button->draw();
}

int getTouchState(touchPosition *touch)
{
	if (keysDown() & KEY_TOUCH)
	{
		touchRead(touch);
		return STATE_TAP;
	}
	if (keysHeld() & KEY_TOUCH)
	{
		touchRead(touch);
		return STATE_HOLD;
	}
	if (keysUp() & KEY_TOUCH)
		return STATE_RELEASE;
	return STATE_NONE;
}

int Menu::activate(bool initial)
{
	int returnVal = -2;
	touchPosition touch;
	swiWaitForVBlank();
	draw(true);
	switch (type)
	{
		case MENU_BUTTON:
		{
			while (returnVal == -2)
			{
				updateSubBG();
				updateFrame();
				updateTime(); //Used to ensure random world seed changes
				scanKeys();
				int state = getTouchState(&touch);
				if (state)
					for (std::vector<UIElement_ptr>::size_type i = 0; i != elements.size(); ++i)
						if (elements[i]->update(state, touch.px, touch.py))
							returnVal = i - 1;
			}
		}
		case MENU_LIST:
		{
			touchPosition touch;
			int maxNameLength = maxStringLength(listItems);
			uint column = 0;
			while (returnVal == -2)
			{
				updateFrame();
				updateSubBG();
				scanKeys();
				if (keysDown() & KEY_TOUCH) //New Press
				{
					touchRead(&touch);
					column = ((touch.py - 8) / 8) + 1 - (frameY + listY);
					if (column <= listItems.size() && column >= 1 && (touch.px + getScrollX()) % 512 >= (frameX + listX + 1) * 8 && (touch.px + getScrollX()) % 512 < (frameX + listX + maxNameLength + 1) * 8)
						for (int i = 0; i < maxNameLength; ++i)
							setSubBgTile(frameX + listX + 1 + i, frameY + listY + column, 60);
				}
				else if (keysHeld() & KEY_TOUCH)
					touchRead(&touch);
				else if (keysUp() & KEY_TOUCH)
				{
					uint newColumn = ((touch.py - 8) / 8) + 1 - (frameY + listY);
					if (column == newColumn && column <= listItems.size() && column >= 1 && (touch.px + getScrollX()) % 512 >= (frameX + listX + 1) * 8 && (touch.px + getScrollX()) % 512 < (frameX + listX + maxNameLength + 1) * 8)
						returnVal = column;
					else //Remove any colored buttons, if any
						drawBoxCenter(frameX + listX + 1, frameY + listY + 1, maxNameLength, listItems.size());
				}
				int state = getTouchState(&touch);
				if (state)
					for (std::vector<UIElement_ptr>::size_type i = 0; i != elements.size(); ++i)
						if (elements[i]->update(state, touch.px, touch.py))
							returnVal = i - 1;
			}
		}
	}
	moveSubBg(0, -512);
	return returnVal;
}

void Menu::setAction(void (*function)(UIElement *element, int data, bool data2), int sendData)
{
	elements.back()->setData = function;
	elements.back()->sendData = sendData;
}