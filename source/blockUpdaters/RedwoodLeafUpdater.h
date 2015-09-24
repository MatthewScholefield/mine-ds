#pragma once
#include "../world.h"
#include "blockUpdater.h"

class RedwoodLeafUpdater : public BlockUpdater
{
public:
	RedwoodLeafUpdater();
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};
