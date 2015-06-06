#pragma once
#include "../world.h"
#include "../blockupdate.h"

class dirtUpdater : public blockUpdater {
public:
	dirtUpdater();
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};