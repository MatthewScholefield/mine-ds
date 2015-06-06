#pragma once
#include "../world.h"
#include "../blockupdate.h"

void leafChanceUpdateGeneric(WorldObject* world, int x, int y, bool bg);

class leafUpdater : public blockUpdater {
public:
	leafUpdater();
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};