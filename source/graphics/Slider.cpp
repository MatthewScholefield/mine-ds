#include "Slider.h"
#include "subBgHandler.h"
#include "../general.h"

bool Slider::isTouching(int touchX, int touchY)
{
	touchY += getScrollY();
	touchX = (touchX + getScrollX()) % 512;
	return isVisible && touchX > (x + tilePos + 2)*8 && touchX <= (x + tilePos + 3)*8 && touchY > (y + 1)*8 && touchY <= (y + 2)*8;
}

void Slider::setVisible(bool visible)
{
	if (visible == isVisible)
		return;
	if (visible)
		draw();
	else
		dragging = false;
	isVisible = visible;
}

void Slider::drawCenterStrip()
{
	for (int i = x + 1; i < x + length - 2; ++i)
	{
		int bgTile = 93;
		if (i == x + 2 + tilePos)
			bgTile = dragging ? 63 : 31;
		setSubBgTile(i, y + 1, bgTile);
	}
	setSubBgTile(x + 1, y + 1, 94);
	setSubBgTile(x + length - 2, y + 1, 94, H_FLIP);
}

void Slider::draw(bool printLabel)
{
	if (printLabel)
		printXY(x, y - 1, label);
	drawBoxFrame(x, y, length, 3);
	drawCenterStrip();
}

bool Slider::update(int state, int touchX, int touchY)
{
	switch (state)
	{
		case STATE_TAP:
			if (isTouching(touchX, touchY))
			{
				dragging = true;
				drawCenterStrip();
			}
			break;
		case STATE_HOLD:
			if (dragging)
			{
				int rawX = touchX / 8 - x - 2;
				if (rawX > length - 5)
					rawX = length - 5;
				else if (rawX < 0)
					rawX = 0;
				if (tilePos != rawX)
				{
					tilePos = rawX;
					drawCenterStrip();
				}
			}
			break;
		case STATE_RELEASE:
			if (dragging)
			{
				dragging = false;
				if (setData)
					setData(this, tilePos, false);
				drawCenterStrip();
			}
			break;
	}
	return false;
}