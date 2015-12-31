#pragma once
#include "../world/World.h"
#include "../blockID.h"
#include "blockUpdater.h"

#define MAX_WATER_LEVEL 12

int getWaterLevel(World &world, int x, int y);
bool isWaterAt(World &world, int px, int py);
int pushWaterFrom(World &world, int x, int y);
void fillBucket(World &world, int x, int y);

class WaterUpdater : public BlockUpdater
{
public:
	WaterUpdater();

	bool update(World &world, int x, int y, bool bg);
	void attemptSpreading(World &world, int x, int y);
	void attemptSharing(World &world, int x, int y);
};

inline int getWaterLevel(World &world, int x, int y)
{
	return world.data[x][y]&0xF;
}

inline void setWaterLevel(World &world, int x, int y, int level)
{
	world.data[x][y] = (world.data[x][y]&0xFFFF0000) | level;
}