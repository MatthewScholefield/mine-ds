#pragma once
#include "../world/World.h"
#include "blockUpdater.h"
#include "../graphics/graphics.h"

class FurnaceUpdater : public BlockUpdater
{
public:
	FurnaceUpdater();
	bool update(World &world, int x, int y, bool bg);
};

