#pragma once
#include <math.h>
#include <string.h>
#include "subBgHandler.h"
#include "UIElement.h"
#include "../general.h"

class Button : public UIElement {
public:
	int printX, printY;
	bool isColored, checkButton;
	int checkSet;

	bool isTouching(int xVal, int yVal);
	void setVisible(bool);
	void setColored(bool);
	void draw(bool printLabel = true);
	bool update(int state, int touchX, int touchY);

	Button(int x, int y, const char * const label, int length = -1, bool isVisible = true, bool checkButton = false, int checkSet = -1) :
	UIElement(x, y, label, (length > 0 ? length : strlen(label) + 2), isVisible, NULL),
	printX(length > 0 ? x + round(double(length) / 2.0 - double(strlen(label)) / 2.0) : x + 1),
	printY(y + 1), isColored(false), checkButton(checkButton), checkSet(checkSet) {
}

	Button(int x, int y, const char * const label, bool isVisible, bool checkButton = false, int checkSet = -1) :
	UIElement(x, y, label, strlen(label) + 2, isVisible, NULL),
	printX(length > 0 ? x + round(double(length) / 2.0 - double(strlen(label)) / 2.0) : x + 1),
	printY(y + 1), isColored(false), checkButton(checkButton), checkSet(checkSet) { }

	~Button() { }
};
