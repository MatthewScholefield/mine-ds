#include "graphics.h"
#include "UI.h"
#include "subBgHandler.h"
#include "Button.h"
#include <string>
#include <sstream>

void startTransition(bool forward)
{
	moveSubBg(forward ? 256 : -256, 0);
}

static void drawBackOffset(int offX, int offY)
{
	int i, j; //They are famous variables :P
	for (i = 7; i < 32; ++i) //Draws dirt Background
		for (j = 0; j < 32; ++j)
			setSubBgTile(j + offX, i + offY, ((i % 2) ? 90 : 122) + j % 2);
	for (i = 0; i < 26; ++i)
		for (j = 0; j < 7; ++j)
			setSubBgTile(i + offX + 3, j + offY, i + (j * 32)); //Draw the MineDS Logo!
	for (i = 0; i < 7; ++i)
		for (j = 0; j < 3; ++j)
			setSubBgTile(j + offX, i + offY, ((i % 2) ? 90 : 122) + j % 2);
	for (i = 0; i < 7; ++i)
		for (j = 29; j < 32; ++j)
			setSubBgTile(j + offX, i + offY, ((i % 2) ? 90 : 122) + j % 2);
}

void drawBackground(bool firstSlot) //Draws dirt background and MineDS Logo
{
	drawBackOffset(firstSlot ? 0 : 32, 0);
}

int createMenu(Button buttons[], int size, bool showBack, int scrollLength)
{
	int start = 0;
	Button back(25, 20, "Back", showBack);
	Button scroll(1, 20, "\x1F", scrollLength > 0);
	touchPosition touch;
	int selected = -1;
	bool chosen = false;

	uint oldKeys = keysHeld();
	while (!chosen)
	{
		updateSubBG();
		updateFrame();
		updateTime(); //Used to ensure random world seed changes
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH))
		{
			touchRead(&touch);
			if (back.isTouching(touch.px, touch.py))
				back.setColored(true);
			else if (scroll.isTouching(touch.px, touch.py))
				scroll.setColored(true);
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
			else if (scroll.isColored && scroll.isTouching(touch.px, touch.py))
			{
				if (getScrollY() < scrollLength - 1)
				{
					moveSubBg(0, scrollLength);
					scroll.label = "\x1E";
					for (int i = 0; i < size; ++i)
						if (buttons[i].printY >= 24)
							buttons[i].printLabel();
				}
				else
				{
					moveSubBg(0, -scrollLength);
					scroll.label = "\x1F";
				}
				scroll.printLabel();
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
				scroll.setColored(false);
			}
		}
		oldKeys = keysHeld();
		touchRead(&touch);
	}
	for (int i = 0; i < size; ++i)
		buttons[i].setColored(false);
	moveSubBg(0, -64);
	return selected + start;
}

bool enableDisableMenu(bool initial)
{
	uint oldKeys;
	touchPosition touch;
	drawBackground();

	clearText(); //Removes All text from the screen
	Button enabled(9, 10, "Enabled", 12);
	Button disabled(9, 15, "Disabled", 12);
	Button done(25, 19, "Done");
	enabled.setColored(initial);
	disabled.setColored(!initial);

	scanKeys();
	touchRead(&touch);
	oldKeys = keysHeld();
	updateFrame();
	bool returnVal = initial;
	while (1)
	{
		updateSubBG();
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			if (enabled.isTouching(touch.px, touch.py))
			{
				returnVal = true;
				enabled.setColored(true);
				disabled.setColored(false);
			}
			else if (disabled.isTouching(touch.px, touch.py))
			{
				returnVal = false;
				enabled.setColored(false);
				disabled.setColored(true);
			}
			done.setColored(done.isTouching(touch.px, touch.py));
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (done.isColored && done.isTouching(touch.px, touch.py))
				return returnVal;
		}
		oldKeys = keysHeld();
		touchRead(&touch);
		updateFrame();
	}
}

int listMenu(int x, int y, int numItems, int maxNameLength)
{
	touchPosition touch;
	drawBackground();
	Button back(25, 19, "Back");

	drawBox(x, y, maxNameLength + 2, numItems + 2);

	scanKeys();
	touchRead(&touch);
	int column = 0;
	while (1)
	{
		updateSubBG();
		scanKeys();
		if (keysHeld() & KEY_TOUCH)
			touchRead(&touch);
		if (keysDown() & KEY_TOUCH) //New Press
		{
			touchRead(&touch);
			column = ((touch.py - 8) / 8) + 1 - y;
			if (column <= numItems && column >= 1 && touch.px >= (x + 1) * 8 && touch.px < (x + maxNameLength + 1) * 8)
				for (int i = 0; i < maxNameLength; ++i)
					setSubBgTile(x + 1 + i, y + column, 60);
			else if (back.isTouching(touch.px, touch.py))
				back.setColored(true);

		}
		else if (keysUp() & KEY_TOUCH)
		{
			int newColumn = ((touch.py - 8) / 8) + 1 - y;
			if (back.isColored && back.isTouching(touch.px, touch.py))
				return 0;
			else if (column == newColumn && column <= numItems && column >= 1 && touch.px >= (x + 1) * 8 && touch.px < (x + maxNameLength + 1) * 8)
				return column;
			else //Remove any colored buttons, if any
			{
				drawBoxCenter(x + 1, y + 1, maxNameLength, numItems);
				back.setColored(false);
			}
		}
		updateFrame();
	}
}

int wordWrap(std::string &message, int maxWordLength)
{
	int lines = 1;
	int prevEndLine = 0;
	unsigned int curEndLine = maxWordLength;
	for (unsigned int i = 0; i < message.length(); ++i)
	{
		if (message[i] == '\n')
		{
			++lines;
			prevEndLine = i;
			curEndLine = i + maxWordLength;
			continue;
		}
		if (i == curEndLine)
		{
			for (int j = curEndLine; j > prevEndLine; --j)
			{
				if (message[j] == ' ')
				{
					i = j;
					message[i] = '\n';
					++lines;
					prevEndLine = i;
					curEndLine = i + maxWordLength;
					break;
				}
				else if (j == prevEndLine + 1)
				{
					message[i] = '\n';
					++lines;
					prevEndLine = i;
					curEndLine = i + maxWordLength;
				}
			}
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

bool createDialog(std::string message, bool cancel, bool firstHalf)
{
	clearText(firstHalf);
	drawBackground(firstHalf);
	const int WIDTH = 24;
	const int HEIGHT = wordWrap(message, WIDTH - 2) + 2;
	const int X = 4 + (firstHalf ? 0 : 32), Y = 11 - HEIGHT / 2;
	drawBox(X, Y, WIDTH, HEIGHT);
	printStringCenter(X + 1, Y + 1, WIDTH - 2, message);
	Button okay(X + (cancel ? WIDTH / 2 - 2 : 1), Y + HEIGHT + 1, "OK", cancel ? 4 : WIDTH - 2);
	Button cancelButton(X + WIDTH - 9, Y + HEIGHT + 1, "Cancel", cancel);
	Button buttons[] = {okay, cancelButton};
	if (createMenu(buttons, cancel ? 2 : 1, false) == 1)
		return true;
	return false;
}