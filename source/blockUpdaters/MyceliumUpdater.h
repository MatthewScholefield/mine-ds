#pragma once
#include "../WorldObject.h"
#include "blockUpdater.h"

class MyceliumUpdater : public BlockUpdater
{
public:
	MyceliumUpdater();
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};

