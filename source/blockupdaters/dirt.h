#pragma once
#include "../world.h"
#include "../blockupdate.h"

class DirtUpdater : public BlockUpdater
{
public:
	DirtUpdater();
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};