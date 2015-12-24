#pragma once
#include "../WorldObject.h"
#include "blockUpdater.h"

class AirUpdater : public BlockUpdater
{
public:
	AirUpdater();
	bool update(WorldObject &world, int x, int y, bool bg);

};
