#include "../../inventory.h"
#include "../../world.h"
#include "../graphics.h"
#include "../../blockID.h"

#pragma once

class InvGfxHandler
{
	Graphic gfx[NUM_INV_SPACES];
	bool loadedGfx[NUM_INV_SPACES];
	int loadedID[NUM_INV_SPACES];

	static void drawSlots(int selectedSlot, int startX, int startY, int xCount, int yCount, int xSpace, int ySpace);
	void update();
public:

	InvGfxHandler() : gfx { }, loadedGfx{}, loadedID{}
	{
		for (auto &i : gfx)
			loadGraphicSub(&i, GRAPHIC_BLOCK, AIR);
	}

	~InvGfxHandler() { }
};