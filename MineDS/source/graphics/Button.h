#pragma once
#include <math.h>
#include "subBgHandler.h"
#include "../general.h"
class Button
{
public:
	int x, y;
	const char *label;
	int length; //Length of button
	int printX, printY;
	bool isColored, visible;

	bool isTouching(int xVal, int yVal);
	void setVisible(bool);
	void setColored(bool);
	void draw();


	Button (int setX,int setY,const char * const setLabel,int setLength = -1,bool isVisible = true) :
		x(setX),y(setY),label(setLabel),length (setLength > 0 ? setLength : strlen(setLabel)+2),
		printX(setLength>0?setX + round(double(setLength) / 2.0 - double(strlen(setLabel)) / 2.0):setX+1),
		printY(setY+1),isColored(false),visible(isVisible)
	{
		if (isVisible)
		{
			drawButton(x, y, length);
			printXY(printX, printY, label);
		}
	}

	Button (int setX,int setY,const char * const setLabel,bool isVisible) :
		x(setX), y(setY), label(setLabel), length (strlen(setLabel)+2),
		printX(setX+1), printY(setY+1), isColored(false),visible(isVisible)
	{
		if (isVisible)
		{
			drawButton(x, y, length);
			printXY(printX, printY, label);
		}
	}
};
