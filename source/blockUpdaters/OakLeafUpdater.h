#pragma once
#include "../world.h"
#include "../blockUpdater.h"

void leafChanceUpdateGeneric(WorldObject &world, int x, int y, bool bg);

class OakLeafUpdater : public BlockUpdater
{
public:
	OakLeafUpdater();
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};