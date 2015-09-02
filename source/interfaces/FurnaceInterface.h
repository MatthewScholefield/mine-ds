#include "Interface.h"
#include "../graphics/Menu.h"
#include "../furnaceHandler.h"
#include "../mining.h"

#pragma once
class FurnaceInterface : public Interface
{
	Menu menu;

	//Buttons
	static const int SMELT = 1;
	static const int BACK = 2;
public:

	void update(WorldObject *world, touchPosition *touch);
	void draw();

	FurnaceInterface() : Interface(INTERFACE_FURNACE), menu(MENU_BUTTON, false)
	{
		menu.addButton(24, 16, "Smelt");
		menu.addButton(1, 16, "Back");
		lcdMainOnTop();
		setMiningDisabled(true);
	}

	~FurnaceInterface()
	{
		closeFurnace();
	}
};