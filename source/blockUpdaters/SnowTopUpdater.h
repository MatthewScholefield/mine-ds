#pragma once
#include "../world.h"
#include "blockUpdater.h"

class SnowTopUpdater : public BlockUpdater
{
public:
	SnowTopUpdater();
	//virtual void chanceUpdate(worldObject* world, int x, int y, bool bg);
	bool update(WorldObject &world, int x, int y, bool bg);
};