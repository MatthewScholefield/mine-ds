#include "WaterMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "mobFunctions.h"
#include "../graphics/3DHandler.h"
#include "../general.h"
#include "../FixedPoint.h"
#include "../blockUpdaters/WaterUpdater.h"

void WaterMob::calcMiscData(World &world)
{
	vy += FixedPoint(true, (18 * FixedPoint::SCALER) / FPS);
	y += (16 * vy) / FPS;
}

void addWater(World &world, int x, int y, int amount)
{
	int totalWater = amount + getWaterLevel(world, x, y);
	if (totalWater <= MAX_WATER_LEVEL)
		setWaterLevel(world, x, y, totalWater);
	else
	{
		if (world.blocks[x][y - 1] !=WATER)
		{
			world.blocks[x][y - 1] = WATER;
			setWaterLevel(world, x, y - 1, totalWater - MAX_WATER_LEVEL);
		}
		else addWater(world,x,y-1,totalWater-MAX_WATER_LEVEL);
		setWaterLevel(world, x, y, MAX_WATER_LEVEL);
	}
	updateAround(world, x, y);
}

void WaterMob::updateMob(World &world)
{
	int blockX = (x + sx / 2) / 16;
	int blockY = (y + 1) / 16;
	switch (world.blocks[blockX][blockY])
	{
	case WATER:
	{
		if (isWaterAt(world, x + sx / 2, y + 1))
		{
			addWater(world, blockX, blockY, level);
			health = 0;
			break;
		}
	}
	case AIR:
	{
		Color color{0, 255, 255};
		if ((world.bgblocks[blockX][(y - sy) / 16] == AIR) && (world.bgblocks[blockX][y / 16] == AIR))
			drawRect(color / (world.bgblocks[blockX][(y - sy) / 16] == AIR ? 2 : 1), x - world.camX, y - sy - world.camY, sx, 16);
		else
		{
			int topSize = 16 ;
			drawRect(color / (world.bgblocks[blockX][(y - sy) / 16] == AIR ? 2 : 1), x - world.camX, y - sy - world.camY, sx, topSize);
			drawRect(color / (world.bgblocks[blockX][y / 16] == AIR ? 2 : 1), x - world.camX, topSize + y  - sy- world.camY, sx, sy - topSize);
		}
		break;
	}
	default:
	{
		while (--blockY >= 0 && world.blocks[blockX][blockY] != AIR && world.blocks[blockX][blockY] != WATER);
		if (blockY >= 0)
		{
			if (world.blocks[blockX][blockY] == WATER)
				addWater(world, blockX, blockY, level);
			else
			{
				world.blocks[blockX][blockY] = WATER;
				setWaterLevel(world, blockX, blockY, level);
				updateAround(world, blockX, blockY);
			}
		}
		else
			createItemMob(blockX, blockY, BUCKET_WATER);
		health = 0;
		break;
	}
	}
}
