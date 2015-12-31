#pragma once
#include "../world/World.h"
#include "blockUpdater.h"

class DirtUpdater : public BlockUpdater
{
public:
	DirtUpdater();
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};