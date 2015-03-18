#pragma once
#include "../world.h"
#include "../blockupdate.h"

class oakSaplingUpdater : public blockUpdater {
public:
	oakSaplingUpdater();
	virtual void chanceUpdate(worldObject* world, int x, int y, bool bg);
};