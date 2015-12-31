#include "Interface.h"
#include "../Menu.h"

#pragma once
class PageInterface : public Interface
{
private:
	Menu menu;

	//Buttons
	static const int LEFT = 1;
	static const int RIGHT = 2;
	static const int BACK = 3;

public:
	void update(World &world, touchPosition &touch);
	void draw();

	PageInterface();
	~PageInterface() { }
private:

};