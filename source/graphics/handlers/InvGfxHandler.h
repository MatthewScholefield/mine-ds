#include "../../inventory.h"
#include "../../world.h"
#include "../graphics.h"
#include "../../blockID.h"

#pragma once

class InvGfxHandler
{
	Inventory const &inv;
	Graphic gfx[NUM_INV_SPACES];
	bool loadedGfx[NUM_INV_SPACES];
	int loadedID[NUM_INV_SPACES];
	int startX;
	int startY;
	int xCount;
	int yCount;
	int xDist;
	int yDist;
public:

	void drawSlots(int selectedSlot);
	void update();
	void drawQuantities();
	
	InvGfxHandler(Inventory const &inv, int startX, int startY, int xCount = 15, int yCount = 2, int xDist = 2, int yDist = 3)
	: inv(inv), gfx { }, loadedGfx{}, loadedID{}, startX(startX), startY(startY)
	, xCount(xCount), yCount(yCount), xDist(xDist), yDist(yDist) { }

	InvGfxHandler() : inv(getInventoryRef()) { }

	InvGfxHandler& operator=(const InvGfxHandler &clone)
	{
		for (int i = 0; i < NUM_INV_SPACES; ++i)
		{
			gfx[i] = clone.gfx[i];
			loadedGfx[i] = clone.loadedGfx[i];
			loadedID[i] = clone.loadedID[i];
		}
		startX = clone.startX;
		startY = clone.startY;
		xCount = clone.xCount;
		yCount = clone.yCount;
		xDist = clone.xDist;
		yDist = clone.yDist;
		return *this;
	}

	~InvGfxHandler() { }
};