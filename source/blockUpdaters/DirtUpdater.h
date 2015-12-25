#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"

class DirtUpdater : public BlockUpdater
{
public:
	DirtUpdater();
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};