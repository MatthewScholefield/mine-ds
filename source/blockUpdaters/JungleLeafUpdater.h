#pragma once
#include "../WorldObject.h"
#include "blockUpdater.h"

class JungleLeafUpdater : public BlockUpdater
{
public:
	JungleLeafUpdater();
	virtual void chanceUpdate(WorldObject &world, int x, int y, bool bg);
};