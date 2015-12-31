#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"

class LadderUpdater : public BlockUpdater
{
public:
	LadderUpdater();
	bool update(World &world, int x, int y, bool bg);
};