#pragma once
#include "../world.h"
#include "../blockupdate.h"

class spruceSaplingUpdater : public blockUpdater {
public:
	spruceSaplingUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};