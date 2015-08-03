#pragma once
#include "../world.h"
#include "../blockUpdater.h"

int getWaterLevel(WorldObject *world, int x, int y);
bool isWaterAt(WorldObject *world, int px, int py);

class WaterUpdater : public BlockUpdater
{
public:
	WaterUpdater();
	virtual void update(WorldObject* world, int x, int y, bool bg);
	void attemptSpreading(WorldObject* world, int x, int y);
	void attemptSharing(WorldObject* world, int x, int y);
};