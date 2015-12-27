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
	static const int ARRANGE_LEFT = 5;
	static const int ARRANGE_RIGHT = 6;

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
	void arrangeItems(bool right);

public:
	static bool touchesInvSlot(const touchPosition &touch);
	static int touchedSlot(const touchPosition &touch);
	
	void update(WorldObject &world, touchPosition &touch);

	void draw();

	InventoryInterface(bool open) : Interface(INTERFACE_INVENTORY)
	, menu(MENU_BUTTON, false), open(open), oldInvSlot(-1), selectedGraphic(GraphicType::BLOCK, AIR, false)
	, loadedGraphic(AIR), backButton(), inv(getInventoryRef()), gfxHandler(getInventoryRef(), 1, 9)
	{
		menu.addButton(1, 16, "Back", open);
		backButton = menu.getBack();
		menu.addButton(8, 16, "Save World");
		menu.addButton(21, 16, "Crafting", isSurvival());
		menu.addButton(21, 16, "Pages", 9, !isSurvival());
		menu.addButton(26, 5, "<");
		menu.addButton(29, 5, ">");
		if (open)
		{
			lcdMainOnTop();
			oldInvSlot = inv.hand;
			inv.hand = -1;
		}
		else
			lcdMainOnBottom();
	}

	~InventoryInterface()
	{
	}
};
