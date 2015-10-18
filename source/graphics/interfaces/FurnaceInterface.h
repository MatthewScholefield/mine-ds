#include "Interface.h"
#include "../Menu.h"
#include "../../furnaceHandler.h"
#include "../../mining.h"
#include "../handlers/InvGfxHandler.h"

#pragma once
class FurnaceInterface : public Interface
{

	enum InvSlot
	{
		NONE = -1,
		SOURCE = 0,
		FUEL = 1,
		RESULT = 2
	};

	Menu menu;

	//Buttons
	static const int SMELT = 1;
	static const int BACK = 2;

	bool invOpen;
	InvGfxHandler gfxHandler;
	InvSlot selectedInvSlot;
	Furnace openFurnace;
	UIElement_ptr smeltButton;
	Graphic gfx[3];


	void updateContents();
	void openInv();
	void closeInv();
	void swapItem(InvBlock &original);
public:

	void update(WorldObject &world, touchPosition &touch);
	void draw();

	FurnaceInterface() : Interface(INTERFACE_FURNACE), menu(MENU_BUTTON, false)
	, invOpen(false), gfxHandler(getInventoryRef(), 1, 9), selectedInvSlot(NONE)
	, openFurnace(false), smeltButton()
	{
		for (int i = 0; i < 3; ++i)
			loadGraphicSub(&gfx[i], GRAPHIC_BLOCK, AIR);
		menu.addButton(24, 17, "Smelt");
		smeltButton = menu.getBack();
		menu.addButton(1, 17, "Back");
		lcdMainOnTop();
		setMiningDisabled(true);
	}

	~FurnaceInterface()
	{
		for (int i = 0; i < 3; ++i)
			unloadGraphic(&gfx[i]);
		closeFurnace();
	}
};