#pragma once
#include "../world.h"
#include "../blockupdate.h"

class GrassUpdater : public BlockUpdater
{
public:
	GrassUpdater();
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};