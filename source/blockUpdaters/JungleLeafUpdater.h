#pragma once
#include "../world/World.h"
#include "blockUpdater.h"

class JungleLeafUpdater : public BlockUpdater
{
public:
	JungleLeafUpdater();
	virtual void chanceUpdate(World &world, int x, int y, bool bg);
};