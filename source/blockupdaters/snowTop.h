#pragma once
#include "../world.h"
#include "../blockupdate.h"

class SnowTopUpdater : public BlockUpdater
{
public:
	SnowTopUpdater();
	//virtual void chanceUpdate(worldObject* world, int x, int y, bool bg);
	virtual void update(WorldObject* world, int x, int y, bool bg);
};