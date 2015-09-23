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

bool isWaterAt(WorldObject &world, int px, int py)
{
	return world.blocks[px / 16][py / 16] == WATER
			&& py % 16 >= (16 - (getWaterLevel(world, px / 16, py / 16)*16) / MAX_WATER_LEVEL);
}

WaterUpdater::WaterUpdater()
{
	chance = 1000;
}

static bool attemptPut(WorldObject &world, int amount, int x, int y)
{
	switch (world.blocks[x][y])
	{
	case AIR:
		world.blocks[x][y] = WATER;
		setWaterLevel(world, x, y, amount);
		return true;
	case WATER:
	{
		int i;
		for (i = y; i >= 0 && world.blocks[x][i] == WATER && getWaterLevel(world, x, i) >= MAX_WATER_LEVEL; --i);
		if (i < 0 || world.blocks[x][i] != WATER)
			return false;
		if (getWaterLevel(world, x, i) + amount > MAX_WATER_LEVEL)
		{
			if (world.blocks[x][i - 1] != AIR)
				return false;
			setWaterLevel(world, x, i, MAX_WATER_LEVEL);
			world.blocks[x][i - 1] = WATER;
			setWaterLevel(world, x, i - 1, getWaterLevel(world, x, i) + amount);
		}
		else
		{
			world.blocks[x][i - 1] = WATER;
			setWaterLevel(world, x, i - 1, amount);
		}
		updateAround(world, x, i - 1);
		return true;
	}
	default:
		break;
	}
	return false;
}

int pushWaterFrom(WorldObject &world, int x, int y)
{
	int level = getWaterLevel(world, x, y);
	int left = level / 2 + level % 2;
	int right = level / 2;
	bool putLeft = attemptPut(world, left, x - 1, y);
	bool putRight = attemptPut(world, left, x + 1, y);
	if (putLeft && putRight)
		return 0;
	if (!putLeft&&!putRight)
		return attemptPut(world, level, x, y - 1) ? 0 : level;

	if (!putLeft && !attemptPut(world, left, x + 1, y))
		level -= attemptPut(world, left, x, y - 1) ? left : 0;
	else
		level -= left;

	if (!putRight && !attemptPut(world, right, x - 1, y + 1))
		level -= attemptPut(world, right, x, y - 1) ? right : 0;
	else
		level -= right;

	return level;
}

static void setWater(WorldObject &world, int x, int y, int amount)
{
	if (world.blocks[x][y] != WATER)
	{
		destroyBlock(world, x, y, false, false);
		world.blocks[x][y] = WATER;
	}
	setWaterLevel(world, x, y, amount);
}

static bool flowDown(WorldObject &world, int x, int y)
{
	if (y + 1 > WORLD_HEIGHT || !isBlockWalkThrough(world.blocks[x][y + 1]))
		return false;
	int level = getWaterLevel(world, x, y);
	switch (world.blocks[x][y + 1])
	{
	case AIR:
		world.blocks[x][y] = AIR;
		world.blocks[x][y + 1] = WATER;
		setWaterLevel(world, x, y + 1, level);
		return true;
	case WATER:
	{
		if (getWaterLevel(world, x, y + 1) == MAX_WATER_LEVEL)
			return false;
		int newLevel = getWaterLevel(world, x, y + 1) + level;
		if (newLevel > MAX_WATER_LEVEL)
		{
			setWaterLevel(world, x, y, newLevel - MAX_WATER_LEVEL);
			setWaterLevel(world, x, y + 1, MAX_WATER_LEVEL);
			return true;
		}
		else
		{
			world.blocks[x][y] = AIR;
			setWaterLevel(world, x, y + 1, newLevel);
			return true;
		}
	}
	default:
		destroyBlock(world, x, y + 1, false, false);
		world.blocks[x][y + 1] = WATER;
		world.blocks[x][y] = AIR;
		setWaterLevel(world, x, y + 1, level);
		return true;
	}
}

bool WaterUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	if (bg == true)
	{
		//Try to not have water in the background
		if (world.blocks[x][y] == AIR)
		{
			world.blocks[x][y] = WATER;
			world.bgblocks[x][y] = AIR;
		}
		return false;
	}

	if (getWaterLevel(world, x, y) == 0)
	{
		world.blocks[x][y] = AIR;
		return true;
	}

	int origLevel = getWaterLevel(world, x, y);
	if (flowDown(world, x, y))
		return true;

	bool leftBound = x > 0;
	bool rightBound = (x + 1) < WORLD_WIDTH;
	bool canMixLeft = leftBound && isBlockWalkThrough(world.blocks[x - 1][y]);
	bool canMixRight = rightBound && isBlockWalkThrough(world.blocks[x + 1][y]);

	int leftLevel = canMixLeft ? getWaterLevel(world, x - 1, y) : origLevel;
	int rightLevel = canMixRight ? getWaterLevel(world, x + 1, y) : origLevel;

	int leftWater = ((leftBound && world.blocks[x - 1][y] == WATER) ? getWaterLevel(world, x - 1, y) : 0);
	int rightWater = ((rightBound && world.blocks[x + 1][y] == WATER) ? getWaterLevel(world, x + 1, y) : 0);
	int total = getWaterLevel(world, x, y) + leftWater + rightWater;
	int shiftedTotal = getWaterLevel(world, x, y)+(leftWater << 4)+(rightWater << 8);

	int div = 1 + canMixLeft + canMixRight;
	if (total < div || div < 2 || abs(leftLevel - origLevel) + abs(rightLevel - origLevel) < 2)
		return false;
	int addAmount = total / div;
	int newLevel = addAmount + (total % div);
	if (total % div == 2)
	{
		++addAmount;
		newLevel -= 2;
	}
	if (canMixLeft)
		setWater(world, x - 1, y, addAmount);
	if (canMixRight)
		setWater(world, x + 1, y, addAmount);
	setWaterLevel(world, x, y, newLevel);
	int newShiftedTotal = getWaterLevel(world, x, y)
			+((canMixLeft ? getWaterLevel(world, x - 1, y) : 0) << 4)
			+((canMixRight ? getWaterLevel(world, x + 1, y) : 0) << 8);
	return shiftedTotal != newShiftedTotal;
}

void fillBucket(WorldObject &world, int x, int y)
{
	int level = getWaterLevel(world, x, y);
	if (level < MAX_WATER_LEVEL)
	{
		if (level + world.reservedWater >= MAX_WATER_LEVEL)
			world.reservedWater -= MAX_WATER_LEVEL - level;
		else
		{
			world.reservedWater += getWaterLevel(world, x, y);
			world.blocks[x][y] = AIR;
			updateAround(world, x, y);
			return;
		}
	}
	world.blocks[x][y] = AIR;
	subInventory(BUCKET_EMPTY, 1);
	addInventory(BUCKET_WATER);
	updateAround(world, x, y);
}