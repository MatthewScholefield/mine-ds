#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"

class SnowTopUpdater : public BlockUpdater
{
public:
	SnowTopUpdater();
	//virtual void chanceUpdate(worldObject* world, int x, int y, bool bg);
	bool update(World &world, int x, int y, bool bg);
};