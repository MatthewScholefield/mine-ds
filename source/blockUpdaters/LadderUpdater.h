#pragma once
#include "../WorldObject.h"
#include "blockUpdater.h"

class LadderUpdater : public BlockUpdater
{
public:
	LadderUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);
};