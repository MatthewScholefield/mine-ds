#include <stdio.h>
#include <string.h>
#include <nds.h>
#include "Button.h"
#include "subBgHandler.h"
#include "../general.h"

void Button::setColored(bool colored)
{
	if (isVisible)
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
	isVisible = setVisible;
}

bool Button::isTouching(int xVal, int yVal)
{
	yVal += getScrollY();
	xVal = (xVal + getScrollX()) % 512;
	return isVisible && (xVal > x * 8 && xVal < (x + length)*8 && yVal > y * 8 && yVal < (y + 3)*8);
}

void Button::draw(bool printLabels)
{
	if (printLabels)
		printXY(printX, printY, label);
	drawButton(x, y, length);
}

bool Button::update(int state, int touchX, int touchY)
{
	if (state == STATE_TAP && isTouching(touchX, touchY))
		setColored(true);
	else if (state == STATE_RELEASE && isColored && isTouching(touchX, touchY))
		return true;
	return false;
}