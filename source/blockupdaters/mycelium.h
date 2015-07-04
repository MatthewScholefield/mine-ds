#pragma once
#include "../world.h"
#include "../blockupdate.h"

class MyceliumUpdater : public BlockUpdater
{
public:
	MyceliumUpdater();
	virtual void chanceUpdate(WorldObject* world, int x, int y, bool bg);
};

