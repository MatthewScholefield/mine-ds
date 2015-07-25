#include <stdio.h>
#include <string.h>
#include <nds.h>
#include "Button.h"
#include "subBgHandler.h"
#include "../general.h"
#include "../sounds.h"

void Button::setColored(bool colored)
{
	if (!isVisible || colored == isColored)
		return;
	isColored = colored;
	draw(false);
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
	if (isColored)
		drawButtonColored(x, y, length);
	else
		drawButton(x, y, length);
	triggerSubBGUpdate();
}

bool Button::update(int state, int touchX, int touchY)
{
	switch (state)
	{
	case STATE_TAP:
		if (isTouching(touchX, touchY))
		{
			setColored(true);
			playSound(SOUND_CLICK);
		}
		break;
	case STATE_RELEASE:
		if (isColored)
		{
			setColored(false);
			if (isTouching(touchX, touchY))
			{
				if (setData)
					setData(this, sendData, false);
				else
					return true;
			}
		}
		break;
	default:
		break;
	}
	return false;
}

void Button::move(int dx, int dy)
{
	x += dx;
	y += dy;
	printX += dx;
	printY += dy;
}