#pragma once
#include "../world.h"
#include "../blockupdate.h"

class RedwoodLeafUpdater : public BlockUpdater
{
public:
	RedwoodLeafUpdater();
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};
