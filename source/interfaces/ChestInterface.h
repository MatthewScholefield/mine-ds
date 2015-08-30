#include "Interface.h"
#include "../inventory.h"
#include "../mainGame.h"
#include "../graphics/Menu.h"
#include "../graphics/graphics.h"
#include "../blockID.h"
#include "../graphics/handlers/InvGfxHandler.h"
#include "../mining.h"

#pragma once

class ChestInterface : public Interface
{
	Menu menu;

	//Buttons
	static const int BACK = 1;
	static const int SAVE = 2;
	static const int CRAFT_MENU = 3;
	static const int PAGE_MENU = 4;

	int oldInvSlot;
	Graphic selectedGraphic;
	int loadedGraphic;
	UIElement_ptr backButton;
	Inventory &inv, *chest;
	InvGfxHandler invHandler, *chestHandler;
	bool selectedChest;

	void updateInv();
	int correctValue(int value);
	void moveSlot(bool right);
	void parseKeyInput();
	void openInventory();
	void closeInventory();
	Inventory &getSelectedInv();
	static bool touchesInvSlot(const touchPosition &touch);
	static int touchedSlot(const touchPosition &touch);
	void parseTouchInput(const touchPosition &touch);
	static void drawHandFrame();

public:
	static void triggerUpdate();
	static void staticUpdate();

	void update(WorldObject *world, touchPosition *touch);

	void draw();

	ChestInterface(bool open) : Interface(INTERFACE_INVENTORY)
	, menu(MENU_BUTTON, false), loadedGraphic(AIR), inv(getInventoryRef())
	, chest(nullptr), invHandler(getInventoryRef(), 1, 9), chestHandler(nullptr)
	{
		loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, AIR);
		menu.addButton(1, 16, "Back");
		backButton = menu.getBack();
		menu.addButton(8, 16, "Save World");
		menu.addButton(21, 16, "Crafting", isSurvival());
		menu.addButton(21, 16, "Pages", 9, !isSurvival());
		lcdMainOnTop();
		setMiningDisabled(true);
		oldInvSlot = inv.hand;
	}

	~ChestInterface()
	{
		if (chestHandler)
			delete chestHandler;
	}
};