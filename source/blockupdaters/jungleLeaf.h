#pragma once
#include "../world.h"
#include "../blockupdate.h"

class JungleLeafUpdater : public BlockUpdater
{
public:
	JungleLeafUpdater();
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};