#pragma once
#include "../world/World.h"
#include "blockUpdater.h"

class MyceliumUpdater : public BlockUpdater
{
public:
	MyceliumUpdater();
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};

