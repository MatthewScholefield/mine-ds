#pragma once
#include "../world.h"
#include "../blockupdate.h"

class jungleLeafUpdater : public blockUpdater {
public:
	jungleLeafUpdater();
	virtual void chanceUpdate(worldObject* world, int x, int y, bool bg);
};