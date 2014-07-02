#include <stdio.h>
#include <string.h>
#include <nds.h>
#include "Button.h"
#include "subBgHandler.h"
#include "../general.h"

int round(double a)
{
	return int(a + 0.5);
}

void Button::initDefault(int setPrintX, int setPrintY, int setX, int setY, int setLength, const char * const setLabel, bool setVisible)
{
	label = setLabel;
	printX = setPrintX;
	printY = setPrintY;
	x = setX;
	y = setY;
	length = setLength;
	isColored = false;
	visible = setVisible;
	if (visible)
	{
		drawButton(x, y, length + 1);
		printXY(printX, printY, label);
	}
}

Button::Button(int setX, int setY, const char * const setLabel)
{
	initDefault(setX + 1, setY + 1, setX, setY, strlen(setLabel), setLabel, true);
}

Button::Button(int setX, int setY, const char * const setLabel, bool setVisible)
{
	initDefault(setX + 1, setY + 1, setX, setY, strlen(setLabel), setLabel, setVisible);
}

Button::Button(int setX, int setY, const char * const setLabel, int customLength)
{
	initDefault(setX + round(double(customLength) / 2.0) - round(double(strlen(setLabel)) / 2.0) + 1, setY + 1, setX, setY, customLength - 1, setLabel, true);
}

Button::Button(int setX, int setY, const char * const setLabel, int customLength, bool setVisible)
{
	initDefault(setX + round(double(customLength) / 2.0) - round(double(strlen(setLabel)) / 2.0) + 1, setY + 1, setX, setY, customLength - 1, setLabel, setVisible);
}

void Button::setColored(bool colored)
{
	if (visible)
	{
		if (colored)
			drawButtonColored(x, y, length + 1);
		else
			drawButton(x, y, length + 1);
		isColored = colored;
	}
}

void Button::setVisible(bool setVisible)
{
	if (setVisible)
	{
		printXY(printX, printY, label);
		drawButton(x, y, length + 1);
	}
	else
		isColored = false;
	visible = setVisible;
}

bool Button::isTouching(int xVal, int yVal)
{
	return visible && (xVal > x * 8 && xVal < (x + length + 2)*8 && yVal > y * 8 && yVal < (y + 3)*8);
}

void Button::draw()
{
	printXY(printX, printY, label);
	drawButton(x, y, length + 1);
}