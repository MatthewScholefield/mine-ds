#pragma once
#include "../world/World.h"
#include "blockUpdater.h"

class RedwoodLeafUpdater : public BlockUpdater
{
public:
	RedwoodLeafUpdater();
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};
