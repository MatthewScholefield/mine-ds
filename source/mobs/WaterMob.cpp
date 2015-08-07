#include "WaterMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "mobFunctions.h"
#include "../graphics/3DHandler.h"
#include "../general.h"
#include "../FixedPoint.h"
#include "../blockUpdaters/water.h"

void WaterMob::calcMiscData(WorldObject* world)
{
	vy += FixedPoint(true, (18 * FixedPoint::SCALER) / FPS);
	y += (16 * vy) / FPS;
}

void WaterMob::updateMob(WorldObject* world)
{
	int blockX = (x + sx / 2) / 16;
	int blockY = (y + 1) / 16;
	switch (world->blocks[blockX][blockY])
	{
	case WATER:
	{
		if (isWaterAt(world, x + sx / 2, y + 1))
		{
			int totalWater = level + getWaterLevel(world, blockX, blockY);
			if (totalWater <= 12)
				world->data[blockX][blockY] |= totalWater;
			else
			{
				world->blocks[blockX][blockY - 1] = WATER;
				world->data[blockX][blockY - 1] |= totalWater - 12;
				world->data[blockX][blockY] |= 12;
			}
			health = 0;
			break;
		}
	}
	case AIR:
	{
		Pair3<int>color(0, 255, 255);
		if ((world->bgblocks[blockX][blockY - 1] == AIR) == (world->bgblocks[blockX][y / 16] == AIR))
			drawRect(color / (world->bgblocks[blockX][blockY - 1] == AIR ? 2 : 1), x - world->camX, y - sy - world->camY, sx, sy);
		else
		{
			int topSize = 16 - (y % 16);
			drawRect(color / (world->bgblocks[blockX][blockY - 1] == AIR ? 2 : 1), x - world->camX, y - sy - world->camY, sx, topSize);
			drawRect(color / (world->bgblocks[blockX][blockY] == AIR ? 2 : 1), x - world->camX, topSize + y - sy - world->camY, sx, sy - topSize);
		}
		break;
	}
	default:
	{
		while (--blockY >= 0 && world->blocks[blockX][blockY] != AIR);
		if (blockY >= 0)
		{
			world->blocks[blockX][blockY] = WATER;
			world->data[blockX][blockY] |= level;
		}
		else
			createItemMob(blockX, blockY, BUCKET_WATER);
		health = 0;
		break;
	}
	}
}