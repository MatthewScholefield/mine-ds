#pragma once
#include "UIElement.h"
#include <string.h>

class CheckButton : public UIElement
{
private:
	const int SIZE = 12;
	const int OFF = 6;
	bool enabled;
	bool isTouching(int touchX, int touchY);
	void drawCheck();
public:
	void setVisible(bool visible);
	void draw(bool printLabel = true);
	bool update(int state, int touchX, int touchY);

	CheckButton(int x, int y, const char * const label, bool enabled, int length = - 1, bool isVisible = true) :
	UIElement(x, y, label, length < 0 ? strlen(label) + 5 : length, isVisible, nullptr), enabled(enabled) { }
};