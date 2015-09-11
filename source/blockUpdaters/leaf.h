#pragma once
#include "../world.h"
#include "../blockUpdater.h"

void leafChanceUpdateGeneric(WorldObject &world, int x, int y, bool bg);

class LeafUpdater : public BlockUpdater
{
public:
	LeafUpdater();
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};