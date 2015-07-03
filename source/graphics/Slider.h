#pragma once
#include "UIElement.h"

class Slider : public UIElement
{
private:
	bool dragging;
	int tilePos;
	bool isTouching(int touchX, int touchY);
	void drawCenterStrip();
public:
	void setVisible(bool visible);
	void draw(bool printLabel = true);
	bool update(int state, int touchX, int touchY);

	Slider(int x, int y, const char * const label, int initPos, int length = 30, bool isVisible = true) :
	UIElement(x, y, label, length, isVisible, nullptr), dragging(false), tilePos(initPos) { }
};