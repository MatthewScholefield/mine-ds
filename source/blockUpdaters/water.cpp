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

int getWaterLevel(WorldObject *world, int x, int y)
{
	return world->data[x][y]&0x0F;
}

bool isWaterAt(WorldObject *world, int px, int py)
{
	return world->blocks[px / 16][py / 16] == WATER
			&& py % 16 >= 16 - (getWaterLevel(world, px / 16, py / 16)*16) / 12;
}

WaterUpdater::WaterUpdater()
{
	blockID = WATER;
	chance = NO_CHANCE;
}

void WaterUpdater::attemptSpreading(WorldObject* world, int x, int y)
{
	int waterLevel = world->data[x][y] & 0xF;
	if (waterLevel > 12) waterLevel = 12;
	bool rightClear = false;
	bool leftClear = false;
	bool rightDownSolid = false;
	bool leftDownSolid = false;
	bool downClear = y < WORLD_HEIGHT - 1 && isBlockWalkThrough(world->blocks[x][y + 1]);
	;
	if (x > 0)
	{
		leftClear = isBlockWalkThrough(world->blocks[x - 1][y]) && world->blocks[x - 1][y] != WATER;
		leftDownSolid = y < WORLD_HEIGHT - 1 && (world->blocks[x - 1][y + 1] == WATER || !isBlockWalkThrough(world->blocks[x - 1][y + 1]));
	}
	if (x < WORLD_WIDTH - 1)
	{
		rightClear = isBlockWalkThrough(world->blocks[x + 1][y]) && world->blocks[x + 1][y] != WATER;
		rightDownSolid = y < WORLD_HEIGHT - 1 && (world->blocks[x + 1][y + 1] == WATER || !isBlockWalkThrough(world->blocks[x + 1][y + 1]));
	}

	//With that out of the way we can check to see if we _can_ spread
	bool spreadRight = false;
	bool spreadLeft = false;
	if (!downClear || world->blocks[x][y + 1] == WATER)
	{
		if (rightClear)
			spreadRight = true;
		if (leftClear)
			spreadLeft = true;
	}
	if (rightClear && rightDownSolid) spreadRight = true;
	if (leftClear && leftDownSolid) spreadLeft = true;

	//Now check that we have enough water for that spreading
	int reqDiv = 1;
	if (spreadLeft) ++reqDiv;
	if (spreadRight) ++reqDiv;
	//If there isn't enough water to spread, don't
	if (waterLevel < reqDiv)
		return;

	//Otherwise, split at will
	int middleLevel = (waterLevel / reqDiv) + (waterLevel % reqDiv);
	int otherLevels = (waterLevel / reqDiv);
	if (spreadLeft)
	{
		destroyBlock(world, x - 1, y, false, false);
		world->blocks[x - 1][y] = WATER;
		world->data[x - 1][y] = (world->data[x - 1][y]&0xFFFF0000) | otherLevels;
	}
	if (spreadRight)
	{
		destroyBlock(world, x + 1, y, false, false);
		world->blocks[x + 1][y] = WATER;
		world->data[x + 1][y] = (world->data[x - 1][y]&0xFFFF0000) | otherLevels;
	}
	//Update this water blocks water level
	world->data[x][y] &= 0xFFFF0000;
	world->data[x][y] |= middleLevel;
}

void WaterUpdater::attemptSharing(WorldObject* world, int x, int y)
{
	int waterLevel = world->data[x][y] & 0xF;
	if (waterLevel > 12) waterLevel = 12;
	bool rightWater = false;
	bool leftWater = false;
	if (x > 0 && world->blocks[x - 1][y] == WATER && world->data[x - 1][y]&0xF)
	{
		leftWater = true;
	}
	if (x < WORLD_WIDTH && world->blocks[x + 1][y] == WATER && world->data[x + 1][y]&0xF)
		rightWater = true;
	if (rightWater)
		waterLevel += world->data[x + 1][y] & 0xF;
	if (leftWater)
		waterLevel += world->data[x - 1][y] & 0xF;
	int reqDiv = 1;
	if (rightWater) ++reqDiv;
	if (leftWater) ++reqDiv;
	int middleLevel = (waterLevel / reqDiv) + (waterLevel % reqDiv);
	int otherLevels = (waterLevel / reqDiv);
	world->data[x][y] = (world->data[x][y] & 0xFFFF0000) | middleLevel;
	if (rightWater)
		world->data[x + 1][y] = (world->data[x + 1][y] & 0xFFFF0000) | otherLevels;
	if (leftWater)
		world->data[x - 1][y] = (world->data[x - 1][y] & 0xFFFF0000) | otherLevels;
}

void WaterUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	if (bg == true)
	{
		//Try to not have water in the background
		if (world->blocks[x][y] == AIR)
		{
			world->blocks[x][y] = WATER;
		}
		world->bgblocks[x][y] = AIR;
	}
	else
	{
		if (getTime() % 8)
			return;
		if (y < (WORLD_HEIGHT - 1) && isBlockWalkThrough(world->blocks[x][y + 1]))
		{
			if (world->blocks[x][y + 1] != WATER)
			{

				destroyBlock(world, x, y + 1, false, false);
				world->blocks[x][y + 1] = WATER;
				world->data[x][y + 1] = (world->data[x][y] & 0xFFFF) | (world->data[x][y + 1] & 0xFFFF0000);
				world->blocks[x][y] = AIR;
			}
			else if (world->blocks[x][y + 1] == WATER)
			{
				int belowWaterLevel = world->data[x][y + 1] & 0xF;
				if (belowWaterLevel > 12) belowWaterLevel = 12;
				int neededWater = 12 - belowWaterLevel;
				if (neededWater == 0)
				{
					attemptSpreading(world, x, y);
					attemptSharing(world, x, y);
				}
				int waterLevel = world->data[x][y] & 0x0F;
				if (waterLevel >= neededWater)
				{
					waterLevel -= neededWater;
					world->data[x][y + 1] = (world->data[x][y + 1] & 0xFFFF0000) | 12;
					world->data[x][y] = (world->data[x][y] & 0xFFFF0000) | waterLevel;
				}
				else if (waterLevel < neededWater)
				{
					world->data[x][y] = world->data[x][y] & 0xFFFF0000;
					world->blocks[x][y] = AIR;
					belowWaterLevel += waterLevel;
					world->data[x][y + 1] = (world->data[x][y + 1] & 0xFFFF0000) | belowWaterLevel;
				}
			}
		}
		else
		{
			attemptSpreading(world, x, y);
			attemptSharing(world, x, y);
		}
	}
	if ((world->data[x][y] & 0x0F) == 0)
		world->blocks[x][y] = AIR;
}