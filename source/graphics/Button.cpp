#include <stdio.h>
#include <string.h>
#include <nds.h>
#include "Button.h"
#include "subBgHandler.h"
#include "../general.h"

int Button::selectedCheck[] = {};
int Button::currentID = 0;

void Button::setColored(bool colored)
{
	if (!isVisible || colored == isColored)
		return;
	if (colored)
		drawButtonColored(x, y, length);
	else
		drawButton(x, y, length);
	isColored = colored;
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
}

bool Button::update(int state, int touchX, int touchY)
{
	if (state == STATE_TAP && isTouching(touchX, touchY))
	{
		if (checkButton)
		{
			if (checkSet != CHECK_SET_NONE)
				selectedCheck[checkSet] = currentID;
			else
				setColored(!checkState);
		}
		else
			setColored(true);
	}
	if (checkButton && isColored && checkSet != CHECK_SET_NONE && currentID != selectedCheck[checkSet])
		setColored(false);
	else if (state == STATE_RELEASE)
	{
		if (isTouching(touchX, touchY))
		{
			if (checkButton)
				checkState = !checkState;
			if (setData)
			{
				setData(this, sendData, checkState);
				if (!checkButton)
					setColored(false);
			}
			else if (!checkButton && isColored)
			{
				setColored(false);
				return true;
			}
		}
		else
			setColored(checkButton ? checkState : false);
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