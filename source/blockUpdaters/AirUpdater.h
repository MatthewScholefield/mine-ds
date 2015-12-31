#pragma once
#include "../world/WorldObject.h"
#include "blockUpdater.h"

class AirUpdater : public BlockUpdater
{
public:
	AirUpdater();
	bool update(World &world, int x, int y, bool bg);

};
