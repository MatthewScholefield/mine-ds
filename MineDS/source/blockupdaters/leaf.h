#pragma once
#include "../world.h"
#include "../blockupdate.h"

void leafChanceUpdateGeneric(worldObject* world, int x, int y, bool bg);

class leafUpdater : public blockUpdater {
public:
	leafUpdater();
	virtual void chanceUpdate(worldObject* world, int x, int y, bool bg);
};