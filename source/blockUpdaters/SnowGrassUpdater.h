#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"

class SnowGrassUpdater : public BlockUpdater
{
public:
	SnowGrassUpdater();
	bool update(World &world, int x, int y, bool bg);
};