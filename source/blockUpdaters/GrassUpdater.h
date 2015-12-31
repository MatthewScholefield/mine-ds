#pragma once
#include "../world/World.h"
#include "blockUpdater.h"

class GrassUpdater : public BlockUpdater
{
public:
	GrassUpdater();
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};