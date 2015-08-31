#include <algorithm>

#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../graphics/3DHandler.h"
#include "../blockUpdater.h"
#include "../blockID.h"
#include "../blocks.h"
#include "water.h"
#include "../mobs/mobHandler.h"
#include "../mining.h"

bool isWaterAt(WorldObject *world, int px, int py)
{
	return world->blocks[px / 16][py / 16] == WATER
			&& py % 16 >= (16 - (getWaterLevel(world, px / 16, py / 16)*16) / 12);
}

WaterUpdater::WaterUpdater()
{
	chance = NO_CHANCE;
}

static void setWater(WorldObject *world, int x, int y, int amount)
{
	if (world->blocks[x][y] != WATER)
	{
		destroyBlock(world, x, y, false, false);
		world->blocks[x][y] = WATER;
	}
	setWaterLevel(world, x, y, amount);
}

static bool flowDown(WorldObject *world, int x, int y)
{
	if (y + 1 > WORLD_HEIGHT || !isBlockWalkThrough(world->blocks[x][y + 1]))
		return false;
	int level = getWaterLevel(world, x, y);
	switch (world->blocks[x][y + 1])
	{
	case AIR:
		world->blocks[x][y] = AIR;
		world->blocks[x][y + 1] = WATER;
		setWaterLevel(world, x, y + 1, level);
		return true;
	case WATER:
	{
		if (getWaterLevel(world, x, y + 1) == 12)
			return false;
		int newLevel = getWaterLevel(world, x, y + 1) + level;
		if (newLevel > 12)
		{
			setWaterLevel(world, x, y, newLevel - 12);
			setWaterLevel(world, x, y + 1, 12);
			return false;
		}
		else
		{
			world->blocks[x][y] = AIR;
			setWaterLevel(world, x, y + 1, newLevel);
			return true;
		}
	}
	default:
		destroyBlock(world, x, y + 1, false, false);
		world->blocks[x][y + 1] = WATER;
		return true;
	}
}

bool WaterUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	if (bg == true)
	{
		//Try to not have water in the background
		if (world->blocks[x][y] == AIR)
		{
			world->blocks[x][y] = WATER;
			world->bgblocks[x][y] = AIR;
		}
		return false;
	}

	if (getWaterLevel(world, x, y) == 0)
	{
		world->blocks[x][y] = AIR;
		return true;
	}

	int previous = getWaterLevel(world, x, y);
	if (flowDown(world, x, y))
		return true;

	bool leftBound = x - 1 > 0, rightBound = x + 1 <= WORLD_WIDTH;
	bool canMixLeft = leftBound && isBlockWalkThrough(world->blocks[x - 1][y]);
	bool canMixRight = rightBound && isBlockWalkThrough(world->blocks[x + 1][y]);

	int total = getWaterLevel(world, x, y)
			+ (leftBound && world->blocks[x - 1][y] == WATER ? getWaterLevel(world, x - 1, y) : 0)
			+ (rightBound && world->blocks[x + 1][y] == WATER ? getWaterLevel(world, x + 1, y) : 0);

	int div = 1 + canMixLeft + canMixRight;
	if (total < div)
		return false;
	int addAmount = total / div;
	if (canMixLeft)
		setWater(world, x - 1, y, addAmount);
	if (canMixRight)
		setWater(world, x + 1, y, addAmount);
	int newLevel = addAmount + (total % div);
	setWaterLevel(world, x, y, newLevel);
	return previous != newLevel;
}
