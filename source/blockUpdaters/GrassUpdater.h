#pragma once
#include "../world.h"
#include "blockUpdater.h"

class GrassUpdater : public BlockUpdater
{
public:
	GrassUpdater();
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};