#pragma once
#include "../world.h"
#include "../blockupdate.h"

class airUpdater : public blockUpdater
{
public:
	airUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);

};
