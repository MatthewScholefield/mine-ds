#pragma once
#include "../world.h"
#include "../blockupdate.h"

class redwoodLeafUpdater : public blockUpdater
{
public:
	redwoodLeafUpdater();
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};
