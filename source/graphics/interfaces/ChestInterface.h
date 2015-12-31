#include "Interface.h"
#include "../../inventory.h"
#include "../../mainGame.h"
#include "../Menu.h"
#include "../graphics.h"
#include "../../blockID.h"
#include "../handlers/InvGfxHandler.h"
#include "../../mining.h"
#include "../../chests.h"
#include "../../mobs/PlayerMob.h"

#include "../../localizations/locale.h"

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
	Inventory &inv, &chest;
	InvGfxHandler invHandler, chestHandler;
	bool selectedChest;

	enum class Transfer
	{
		TOUCH,
		FULL_JUMP,
		HALF_JUMP,
		SINGLE_JUMP,
		REVERSE_SINGLE_JUMP
	};

	void updateInv();
	int correctValue(int value);
	void moveSlot(bool right);
	void parseKeyInput();
	void openInventory();
	void closeInventory(World &world);
	Inventory &getSelectedInv(int useChest = -1);
	static bool touchesInvSlot(const touchPosition &touch);
	static bool touchesChestSlot(const touchPosition &touch);
	static int touchedSlot(const touchPosition &touch, int yOffset);
	void parseTouchInput(const touchPosition &touch);
	static void drawHandFrame();
	void swapBlocks(int destSlot, bool touchedChest);
	int getAmount(Transfer type, int touched, bool touchedChest);
	void jumpTransfer(int amount, int touched, bool touchedChest);

public:
	static void triggerUpdate();
	static void staticUpdate();

	void update(World &world, touchPosition &touch);
	void draw();

	ChestInterface(World &world, bool open) : Interface(INTERFACE_INVENTORY)
	, menu(MENU_BUTTON, false), oldInvSlot(getInventoryRef().hand), selectedGraphic(GraphicType::BLOCK, AIR, false)
	, loadedGraphic(AIR), backButton(), inv(getInventoryRef()), chest(world.chests[getOpenedChestID()])
	, invHandler(getInventoryRef(), 1, 9), chestHandler(world.chests[getOpenedChestID()], 1, 1)
	, selectedChest(false)
	{
		menu.addButton(1, 16, locale("back"));
		backButton = menu.getBack();
		menu.addButton(8, 16, "Save World");
		menu.addButton(21, 16, "Crafting", isSurvival());
		menu.addButton(21, 16, "Pages", 9, !isSurvival());
		lcdMainOnTop();
		setMiningDisabled(true);
		PlayerMob::setControlsEnabled(false);
		inv.hand = -1;
		updateInv();
	}

	~ChestInterface()
	{
		closeChest();
		PlayerMob::setControlsEnabled(true);
	}
};