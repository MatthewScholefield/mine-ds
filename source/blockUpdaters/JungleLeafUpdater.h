#pragma once
#include "../world.h"
#include "../blockUpdater.h"

class JungleLeafUpdater : public BlockUpdater
{
public:
	JungleLeafUpdater();
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};