#pragma once
#include "../world.h"
#include "../blockID.h"
#include "../blockUpdater.h"

#define MAX_WATER_LEVEL 12

int getWaterLevel(WorldObject &world, int x, int y);
bool isWaterAt(WorldObject &world, int px, int py);
int pushWaterFrom(WorldObject &world, int x, int y);
void fillBucket(WorldObject &world, int x, int y);

class WaterUpdater : public BlockUpdater
{
public:
	WaterUpdater();

	bool update(WorldObject &world, int x, int y, bool bg);
	void attemptSpreading(WorldObject &world, int x, int y);
	void attemptSharing(WorldObject &world, int x, int y);
};

inline int getWaterLevel(WorldObject &world, int x, int y)
{
	return world.data[x][y]&0xF;
}

inline void setWaterLevel(WorldObject &world, int x, int y, int level)
{
	world.data[x][y] = (world.data[x][y]&0xFFFF0000) | level;
}