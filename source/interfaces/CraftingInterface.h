#include "Interface.h"
#include "../graphics/Menu.h"
#include "../crafting.h"

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

	static const int NUM_RECIPES = 26;
	static const Recipe recipes[NUM_RECIPES];
	Graphic resultBlock;
	Graphic neededblocks[4];

	void updateCraftingGraphics();
	void moveCraftingPage(int dir);
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

	CraftingInterface() : Interface(INTERFACE_CRAFTING)
	{
		for (page = 0; !canCraftRecipe(page) && page < NUM_RECIPES; ++page);
		if (page == NUM_RECIPES)
			page = 0;
	}

	~CraftingInterface() { }
};