#include "Interface.h"
#include "../../inventory.h"
#include "../../mainGame.h"
#include "../Menu.h"
#include "../graphics.h"
#include "../../blockID.h"
#include "../handlers/InvGfxHandler.h"

#pragma once

class InventoryInterface : public Interface
{
	Menu menu;

	//Buttons
	static const int BACK = 1;
	static const int SAVE = 2;
	static const int CRAFT_MENU = 3;
	static const int PAGE_MENU = 4;

	bool open;
	int oldInvSlot;
	Graphic selectedGraphic;
	int loadedGraphic;
	UIElement_ptr backButton;
	Inventory &inv;
	InvGfxHandler gfxHandler;

	void updateInv();
	static void checkLimits(int &value);
	void moveSlot(bool right);
	void parseKeyInput();
	void openInventory();
	void closeInventory();
	void switchInvState();
	void parseTouchInput(const touchPosition &touch);
	static void drawHandFrame();

public:
	static bool touchesInvSlot(const touchPosition &touch);
	static int touchedSlot(const touchPosition &touch);
	
	void update(WorldObject &world, touchPosition &touch);

	void draw();

	InventoryInterface(bool open) : Interface(INTERFACE_INVENTORY)
	, menu(MENU_BUTTON, false), open(open), loadedGraphic(AIR)
	, inv(getInventoryRef()), gfxHandler(getInventoryRef(), 1, 9)
	{
		loadGraphicSub(&selectedGraphic, GRAPHIC_BLOCK, AIR);
		menu.addButton(1, 16, "Back");
		backButton = menu.getBack();
		menu.addButton(8, 16, "Save World");
		menu.addButton(21, 16, "Crafting", isSurvival());
		menu.addButton(21, 16, "Pages", 9, !isSurvival());
	}

	~InventoryInterface() { }
};