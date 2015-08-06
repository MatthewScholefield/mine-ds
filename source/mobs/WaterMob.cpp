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
	int blockX = (x) / 16;
	int blockY = (y + sy / 2) / 16;
	switch (world->blocks[blockX][blockY])
	{
	case WATER:
	{
		if (isWaterAt(world, x, y + 9))
		{
			int bottomY = (y + 9) / 16;
			world->blocks[blockX][bottomY - 1] = WATER;
			world->data[blockX][bottomY - 1] |= getWaterLevel(world, blockX, blockY);
			world->blocks[blockX][bottomY] = WATER;
			world->data[blockX][bottomY] |= 12;
			health = 0;
			break;
		}
	}
	case AIR:
	{
		Pair3<int>color(0, 255, 255);
		if ((world->bgblocks[blockX][blockY - 1] == AIR) == (world->bgblocks[blockX][blockY] == AIR))
			drawRect(color / (world->bgblocks[blockX][blockY - 1] == AIR ? 2 : 1), x - 8 - world->camX, y - 8 - world->camY, 16, 16);
		else
		{
			int topSize = 16 - ((y + 8) % 16);
			drawRect(color / (world->bgblocks[blockX][blockY - 1] == AIR ? 2 : 1), x - 8 - world->camX, y - 8 - world->camY, 16, topSize);
			drawRect(color / (world->bgblocks[blockX][blockY] == AIR ? 2 : 1), x - 8 - world->camX, topSize + y - 8 - world->camY, 16, 16 - topSize);
		}
		break;
	}
	default:
	{
		while (blockY >= 0 && world->blocks[blockX][blockY] != AIR)
			--blockY;
		if (blockY >= 0)
		{
			world->blocks[blockX][blockY] = WATER;
			world->data[blockX][blockY] |= 12;
		}
		else
			createItemMob(blockX, blockY, BUCKET_WATER);
		health = 0;
		break;
	}
	}
}