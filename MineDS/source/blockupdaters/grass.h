#pragma once
#include "../world.h"
#include "../blockupdate.h"

class grassUpdater : public blockUpdater {
public:
	grassUpdater();
	virtual void chanceUpdate(worldObject* world, int x, int y, bool bg);
};