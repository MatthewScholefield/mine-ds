#include "Interface.h"
#include "../Menu.h"
#include "../../Recipe.h"
#include "../graphics.h"
#include "../../mobs/PlayerMob.h"
#include "../../localizations/locale.h"
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

	static const int NUM_RECIPES = 44;
	static const Recipe recipes[NUM_RECIPES];
	Graphic resultBlock;
	Graphic neededblocks[4];
	Graphic toolBlockGfx;
	const bool tableInUse;

	void updateCraftingGraphics();
	void moveCraftingPage(bool right);
	void craftItem();
	bool canCraftRecipe(int recipe);
protected:

	virtual bool closeOnMovement()
	{
		return false;
	}
public:

	void update(World &world, touchPosition &touch);
	void draw();

	CraftingInterface(bool tableInUse) : Interface(INTERFACE_CRAFTING)
	, menu(MENU_BUTTON, false), page(0), resultBlock(), neededblocks { }
, toolBlockGfx(GraphicType::BLOCK, tableInUse ? CRAFTING_TABLE : AIR), tableInUse(tableInUse)
	{
		while (!canCraftRecipe(page) && page < NUM_RECIPES)
			++page;
		if (page == NUM_RECIPES)
			page = 0;
		updateCraftingGraphics();
		menu.addButton(0, 10, "\x011");
		menu.addButton(29, 10, "\x010");
		menu.addButton(22, 16, "Craft");
		menu.addButton(3, 16, locale("back"));
	}

	~CraftingInterface()
	{
		if (tableInUse)
			PlayerMob::setControlsEnabled(true);
	}
};
