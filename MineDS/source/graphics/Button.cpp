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

Button::Button(int xLoc, int yLoc, const char * const label)
{
	x = xLoc;
	y = yLoc;
	length = strlen(label);
	drawButton(x, y, length + 1);
	printXY(x + 1, y + 1, label);
	setDefault();
}

Button::Button(int xLoc, int yLoc, const char * const label, int customLength)
{
	x = xLoc;
	y = yLoc;
	length = customLength - 1;
	drawButton(x, y, length + 1);
	printXY(x + round(double(length) / 2.0) - round(double(strlen(label)) / 2.0) + 1, y + 1, label);
	setDefault();
}

void Button::setDefault()
{
	isColored = false;
}

void Button::setColored(bool colored)
{
	if (colored)
	{
		drawButtonColored(x, y, length + 1);
	}
	else
	{
		drawButton(x, y, length + 1);

	}
	isColored = colored;
	//drawButtonColored(x, y, length + 1);
}

bool Button::isTouching(touchPosition touch)
{
	return (touch.px > x * 8 && touch.px < (x + length + 2)*8 && touch.py > y * 8 && touch.py < (y + 3)*8);
}