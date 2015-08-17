#include "Interface.h"
#include "../graphics/Menu.h"
#include "../Recipe.h"
#include "../graphics/graphics.h"

#pragma once

class CraftingInterface : public Interface
{
	Menu menu;
	int page;

	//Buttons
	static const int LEFT = 1;
	static const int RIGHT = 2;
	static const int CRAFT = 3;
	static const int BACK = 4;

	static const int NUM_RECIPES = 27;
	static const Recipe recipes[NUM_RECIPES];
	Graphic resultBlock;
	Graphic neededblocks[4];

	void updateCraftingGraphics();
	void moveCraftingPage(bool right);
	void craftItem();
	static bool canCraftRecipe(int recipe);
protected:

	virtual bool closeOnMovement()
	{
		return false;
	}
public:

	void update(WorldObject *world, touchPosition *touch);
	void draw();

	CraftingInterface() : Interface(INTERFACE_CRAFTING), menu(MENU_BUTTON, false)
	{
		for (page = 0; !canCraftRecipe(page) && page < NUM_RECIPES; ++page);
		if (page == NUM_RECIPES)
			page = 0;
		updateCraftingGraphics();
		menu.addButton(0, 10, "\x011");
		menu.addButton(29, 10, "\x010");
		menu.addButton(22, 16, "Craft");
		menu.addButton(3, 16, "Back");
	}

	~CraftingInterface() { }
};