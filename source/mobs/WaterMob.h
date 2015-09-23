
#include "BaseMob.h"
#include "../blockUpdaters/water.h"

#pragma once

class WaterMob : public BaseMob
{
	int level;
public:
	void updateMob(WorldObject &world);
	void calcMiscData(WorldObject &world);
  void hurt(int,int) {}
	WaterMob(int x, int y, int level) : BaseMob(MOB_WATER, x, y, 16, (level * 16) / MAX_WATER_LEVEL), level(level){ }

	~WaterMob() { }
};
