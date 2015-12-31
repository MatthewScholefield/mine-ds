#pragma once
#include "../world/World.h"
#include "blockUpdater.h"

class JungleGrassUpdater : public BlockUpdater
{
public:
	JungleGrassUpdater();
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};

