#pragma once
#include "../world.h"
#include "../blockupdate.h"

class ladderUpdater : public blockUpdater {
public:
	ladderUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
};