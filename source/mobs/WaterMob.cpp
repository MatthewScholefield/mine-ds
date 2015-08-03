#include "WaterMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "mobFunctions.h"
#include "../graphics/3DHandler.h"
#include "../general.h"
#include "../FixedPoint.h"

void WaterMob::calcMiscData(WorldObject* world)
{
	vy += FixedPoint(true, (18 * FixedPoint::SCALER) / FPS);
	y += (16 * vy) / FPS;
}

void WaterMob::updateMob(WorldObject* world)
{
	if (world->blocks[(x) / 16][(y + sy / 2) / 16] != AIR)
	{
		int x = (this->x) / 16;
		int y = (this->y + sy / 2) / 16;
		while (y >= 0 && world->blocks[x][y] != AIR)
			--y;
		if (y >= 0)
		{
			world->blocks[x][y] = WATER;
			world->data[x][y] |= 12;
		}
		else
			createItemMob(x, this->y, BUCKET_WATER);
		health = 0;
	}
	drawRect(Pair3<int>(0, 192, 255), x - 7 - world->camX, y - 7 - world->camY, 16, 16);
}