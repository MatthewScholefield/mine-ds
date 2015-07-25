#pragma once

#define STATE_NONE 0
#define STATE_TAP 1
#define STATE_HOLD 2
#define STATE_RELEASE 3

class UIElement
{
protected:
public:
	void (*setData)(UIElement *element, int data, bool data2);
	int sendData;
	int x, y;
	const char *label;
	int length; //Length of Element
	bool isVisible;

	virtual void setVisible(bool visible) = 0;
	virtual void draw(bool printLabel = true) = 0;
	virtual bool update(int state, int touchX, int touchY) = 0; //Returns true to exit/move forward
	virtual void move(int dx, int dy);

	UIElement(int x, int y, const char * const label, int length, bool isVisible, void (*setData)(UIElement *element, int data, bool data2)) :
	setData(setData), sendData(0), x(x), y(y), label(label), length(length), isVisible(isVisible) { }

	virtual ~UIElement() { }
};