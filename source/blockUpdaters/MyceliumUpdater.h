#pragma once
#include "../world.h"
#include "blockUpdater.h"

class MyceliumUpdater : public BlockUpdater
{
public:
	MyceliumUpdater();
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};

