#pragma once

#define STATE_TAP 0
#define STATE_HOLD 1
#define STATE_RELEASE 2

class UIElement {
private:
	void (*setData)(int data);
public:
	int x, y;
	const char *label;
	int length; //Length of Element
	bool isVisible;

	virtual void setVisible(bool) = 0;
	virtual void draw(bool printLabel) = 0;
	virtual bool update(int state, int touchX, int touchY) = 0; //Returns true to exit/move forward

	UIElement(int x, int y, const char * const label, int length, bool isVisible, void (*setData)(int data)) :
	setData(setData), x(x), y(y), label(label), length(length), isVisible(isVisible) { }

	virtual ~UIElement() { }
};