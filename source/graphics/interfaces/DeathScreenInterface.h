#include "Interface.h"
#include "../Menu.h"
#include "../../mining.h"
#include "../UI.h"

#pragma once

class DeathScreenInterface : public Interface
{
private:
	Menu menu;

	//Buttons
	static const int RESPAWN = 1;
	static const int TITLESCREEN = 2;
public:
	void update(WorldObject &world, touchPosition &touch);
	void draw();

	DeathScreenInterface() : Interface(INTERFACE_DEATH_SCREEN), menu(MENU_BUTTON, false)
	{
		menu.addButton(8, 10, locale("respawn"), 14);
		menu.addButton(8, 15, locale("main-menu"), 14);
		setMiningDisabled(true);
		lcdMainOnTop();
	}

	~DeathScreenInterface()
	{
		setMiningDisabled(false);
	}
};