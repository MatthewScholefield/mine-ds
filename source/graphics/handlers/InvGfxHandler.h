#include "../../inventory.h"
#include "../../world/World.h"
#include "../graphics.h"
#include "../../blockID.h"

#pragma once

class InvGfxHandler
{
	Inventory const &inv;
	Graphic gfx[NUM_INV_SPACES];
	int loadedID[NUM_INV_SPACES];
	int startX;
	int startY;
	int xCount;
	int yCount;
	int xDist;
	int yDist;
public:

	void drawSlots(bool drawSelected);
	void update();
	void drawQuantities();
	
	InvGfxHandler(Inventory const &inv, int startX, int startY, int xCount = 15, int yCount = 2, int xDist = 2, int yDist = 3)
	: inv(inv), gfx { }, loadedID{}, startX(startX), startY(startY)
	, xCount(xCount), yCount(yCount), xDist(xDist), yDist(yDist) { }

	~InvGfxHandler()
	{
	}
};