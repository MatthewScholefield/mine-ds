#pragma once
#include "../world.h"
#include "../blockUpdater.h"

class LadderUpdater : public BlockUpdater
{
public:
	LadderUpdater();
	bool update(WorldObject* world, int x, int y, bool bg);
};