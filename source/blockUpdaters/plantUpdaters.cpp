#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "plantUpdaters.cpp.h"
#include "../mobs/hurt.h"
#include "../mobs/mobHandler.h"

void plantUpdate(int beneathID, WorldObject &world, int x, int y, bool bg)
{
	int &blockBelowXY = bg ? world.bgblocks[x][y + 1] : world.blocks[x][y + 1];
	if (blockBelowXY != beneathID)
	{
		int &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
		createItemMob(x, y, getSurvivalItem(blockXY));
		blockXY = AIR;
	}
}

ShrubUpdater::ShrubUpdater()
{
}

bool ShrubUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	plantUpdate(SAND, world, x, y, bg);
	return false;
}

TallGrassUpdater::TallGrassUpdater()
{
}

bool TallGrassUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
	return false;
}

RedFlowerUpdater::RedFlowerUpdater()
{
}

bool RedFlowerUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
	return false;
}

YellowFlowerUpdater::YellowFlowerUpdater()
{
}

bool YellowFlowerUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
	return false;
}

RedMushroomUpdater::RedMushroomUpdater()
{
}

bool RedMushroomUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	plantUpdate(MYCELIUM, world, x, y, bg);
	return false;
}

BrownMushroomUpdater::BrownMushroomUpdater()
{
}

bool BrownMushroomUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	plantUpdate(MYCELIUM, world, x, y, bg);
	return false;
}