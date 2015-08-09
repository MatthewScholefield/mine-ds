
#include "BaseMob.h"

#pragma once

class WaterMob : public BaseMob
{
	int level;
public:
	void updateMob(WorldObject* world);
	void calcMiscData(WorldObject* world);
  void hurt(int,int) {}
	WaterMob(int x, int y, int level) : BaseMob(MOB_WATER, x, y, 16, (level * 16) / 12), level(level){ }

	~WaterMob() { }
};
