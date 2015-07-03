#include "CheckButton.h"
#include "subBgHandler.h"
#include "../general.h"

bool CheckButton::isTouching(int touchX, int touchY)
{
	touchY += getScrollY();
	touchX = (touchX + getScrollX()) % 512;
	return isVisible && x * 8 + OFF < touchX && touchX <= x * 8 + OFF + SIZE && y * 8 + OFF < touchY && y * 8 + OFF + SIZE >= touchY;
}

void CheckButton::setVisible(bool visible)
{
	if (isVisible == visible)
		return;
	draw();
	isVisible = visible;
}

void CheckButton::drawCheck()
{
	setSubBgTile(x + 1, y + 1, enabled ? 157 : 28);
}

void CheckButton::draw(bool printLabel)
{
	drawButton(x + 3, y, length - 4);
	setSubBgTile(x + 3, y, 30);
	setSubBgTile(x + 3, y + 2, 30, V_FLIP);
	setSubBgTile(x + 3, y + 1, 28);

	//Draw Borders
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (i == 1 && j == 1)
				continue;
			else if (j == 2)
				setSubBgTile(x + i, y + j, 124 + i, V_FLIP);
			else
				setSubBgTile(x + i, y + j, 124 + i + j * 32);
	drawCheck();
	if (printLabel)
		printXY(x + 3, y + 1, label);
}

bool CheckButton::update(int state, int touchX, int touchY)
{
	if (state == STATE_TAP && isTouching(touchX, touchY))
	{
		enabled = !enabled;
		drawCheck();
		setData(this, sendData, enabled);
	}
	return false;
}