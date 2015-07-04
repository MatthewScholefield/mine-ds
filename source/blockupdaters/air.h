#pragma once
#include "../world.h"
#include "../blockupdate.h"

class AirUpdater : public BlockUpdater
{
public:
	AirUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);

};
