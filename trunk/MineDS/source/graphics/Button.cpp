#include <stdio.h>
#include <string.h>
#include <nds.h>
#include "Button.h"
#include "subBgHandler.h"
#include "../general.h"

void Button::setColored(bool colored)
{
	if (visible)
	{
		if (colored)
			drawButtonColored(x, y, length);
		else
			drawButton(x, y, length);
		isColored = colored;
	}
}

void Button::setVisible(bool setVisible)
{
	if (setVisible)
	{
		printXY(printX, printY, label);
		drawButton(x, y, length);
	}
	else
		isColored = false;
	visible = setVisible;
}

bool Button::isTouching(int xVal, int yVal)
{
	return visible && (xVal > x * 8 && xVal < (x + length)*8 && yVal > y * 8 && yVal < (y + 3)*8);
}

void Button::draw()
{
	printXY(printX, printY, label);
	drawButton(x, y, length);
}