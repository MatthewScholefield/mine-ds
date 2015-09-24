#pragma once
#include "../world.h"
#include "blockUpdater.h"

class SnowGrassUpdater : public BlockUpdater
{
public:
	SnowGrassUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
};