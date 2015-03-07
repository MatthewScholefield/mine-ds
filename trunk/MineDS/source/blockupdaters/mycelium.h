#pragma once
#include "../world.h"
#include "../blockupdate.h"

class myceliumUpdater : public blockUpdater {
public:
	myceliumUpdater();
	virtual void chanceUpdate(worldObject* world, int x, int y, bool bg);
};

