
#include "BaseMob.h"

#pragma once

class WaterMob : public BaseMob
{
public:
	void updateMob(WorldObject* world);
	void calcMiscData(WorldObject* world);

	WaterMob(int x, int y) : BaseMob(MOB_WATER, x, y, 16, 16) { }

	~WaterMob() { }
};