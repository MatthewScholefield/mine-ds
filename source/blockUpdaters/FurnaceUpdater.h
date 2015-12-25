#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"
#include "../graphics/graphics.h"

class FurnaceUpdater : public BlockUpdater
{
public:
	FurnaceUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
};

