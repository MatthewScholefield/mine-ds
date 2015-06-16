#include "graphics.h"
#include "UI.h"
#include "subBgHandler.h"
#include "Button.h"
#include <string>
#include <sstream>

void drawBackground() //Draws dirt background and MineDS Logo
{
	int i, j; //They are famous variables :P
	for (i = 7; i <= 24; ++i) //Draws dirt Background
		for (j = 0; j <= 31; ++j)
			setSubBgTile(j, i, ((i % 2) ? 90 : 122) + j % 2);
	for (i = 0; i <= 25; ++i)
		for (j = 0; j <= 6; ++j)
			setSubBgTile(i + 3, j, i + (j * 32)); //Draw the MineDS Logo!
	for (i = 0; i <= 6; ++i)
		for (j = 0; j < 3; ++j)
			setSubBgTile(j, i, ((i % 2) ? 90 : 122) + j % 2);
	for (i = 0; i <= 6; ++i)
		for (j = 29; j <= 31; ++j)
			setSubBgTile(j, i, ((i % 2) ? 90 : 122) + j % 2);
}

int menu(Button buttons[], int size, bool showBack)
{
	int start = 0;
	Button back(25, 19, "Back", showBack);
	touchPosition touch;
	int selected = -1;
	bool chosen = false;

	uint oldKeys = keysHeld();
	while (!chosen)
	{
		updateFrame();
		updateTime(); //Used to ensure random world seed changes
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH))
		{
			touchRead(&touch);
			if (back.isTouching(touch.px, touch.py))
				back.setColored(true);
			for (int i = 0; i < size; ++i)
				if (buttons[i].isTouching(touch.px, touch.py))
					buttons[i].setColored(true);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (back.isColored && back.isTouching(touch.px, touch.py))
			{
				selected = 0;
				chosen = true;
			}
			for (int i = 0; i < size; ++i)
			{
				if (buttons[i].isColored && buttons[i].isTouching(touch.px, touch.py))
				{
					selected = i + 1;
					chosen = true;
				}
			}
			if (!chosen) //Redraw buttons
			{
				back.setColored(false);
				for (int i = 0; i < size; ++i)
					buttons[i].setColored(false);
			}
		}
		oldKeys = keysHeld();
		touchRead(&touch);
	}
	for (int i = 0; i < size; ++i)
		buttons[i].setColored(false);
	return selected + start;
}

int wordWrap(std::string &message, size_t maxWordLength)
{
	int lines = 1;
	size_t currentWidth = maxWordLength;
	while (currentWidth < message.length())
	{
		std::string::size_type spacePosition = message.rfind(' ', currentWidth);
		if (spacePosition == std::string::npos)
			spacePosition = message.find(' ', currentWidth);
		if (spacePosition != std::string::npos)
		{
			message[ spacePosition ] = '\n';
			currentWidth = spacePosition + maxWordLength + 1;
			++lines;
		}
	}
	return lines;
}

void printStringCenter(int x, int y, int width, std::string message)
{
	std::istringstream stream(message);
	std::string line;
	int lines = 0;
	while (std::getline(stream, line))
	{
		printXY(x + (width - line.length()) / 2, y + lines, line.c_str());
		++lines;
	}
}

bool createDialog(std::string message, bool cancel)
{
	consoleClear();
	drawBackground();
	const int WIDTH = 24;
	const int HEIGHT = wordWrap(message, WIDTH - 2) + 2;
	const int X = 4, Y = 11 - HEIGHT / 2;
	drawBox(X, Y, WIDTH, HEIGHT);
	printStringCenter(X + 1, Y + 1, WIDTH - 2, message);
	Button okay(X + (cancel ? WIDTH / 2 - 2 : 1), Y + HEIGHT + 1, "OK", cancel ? 4 : WIDTH - 2);
	Button cancelButton(X + WIDTH - 9, Y + HEIGHT + 1, "Cancel", cancel);
	Button buttons[] = {okay, cancelButton};
	if (menu(buttons, cancel ? 2 : 1, false) == 1)
		return true;
	return false;
}