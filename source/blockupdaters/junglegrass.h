#pragma once
#include "../world.h"
#include "../blockupdate.h"

class JungleGrassUpdater : public BlockUpdater
{
public:
	JungleGrassUpdater();
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};

