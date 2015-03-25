#pragma once
#include "../world.h"
#include "../blockupdate.h"

class jungleSaplingUpdater : public blockUpdater {
public:
	jungleSaplingUpdater();
	virtual void update(worldObject* world, int x, int y, bool bg);
	virtual void chanceUpdate(worldObject* world, int x, int y, bool bg);
};