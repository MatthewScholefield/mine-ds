
#include "BaseMob.h"
#include "../blockUpdaters/WaterUpdater.h"

#pragma once

class WaterMob : public BaseMob
{
	int level;
public:
	void updateMob(World &world);
	void calcMiscData(World &world);
  void hurt(int,int) {}
	WaterMob(int x, int y, int level) : BaseMob(MOB_WATER, x, y, 16, (level * 16) / MAX_WATER_LEVEL), level(level){ }

	~WaterMob() { }
};
