#include "../world/World.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "plantUpdaters.cpp.h"
#include "../mobs/hurt.h"
#include "../mobs/mobHandler.h"

void plantUpdate(int beneathID, World &world, int x, int y, bool bg)
{
	short &blockBelowXY = bg ? world.bgblocks[x][y + 1] : world.blocks[x][y + 1];
	if (blockBelowXY != beneathID)
	{
		short &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
		createItemMob(x, y, getSurvivalItem(blockXY));
		blockXY = AIR;
	}
}

ShrubUpdater::ShrubUpdater()
{
}

bool ShrubUpdater::update(World &world, int x, int y, bool bg)
{
	plantUpdate(SAND, world, x, y, bg);
	return false;
}

TallGrassUpdater::TallGrassUpdater()
{
}

bool TallGrassUpdater::update(World &world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
	return false;
}

RedFlowerUpdater::RedFlowerUpdater()
{
}

bool RedFlowerUpdater::update(World &world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
	return false;
}

YellowFlowerUpdater::YellowFlowerUpdater()
{
}

bool YellowFlowerUpdater::update(World &world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
	return false;
}

RedMushroomUpdater::RedMushroomUpdater()
{
}

bool RedMushroomUpdater::update(World &world, int x, int y, bool bg)
{
	plantUpdate(MYCELIUM, world, x, y, bg);
	return false;
}

BrownMushroomUpdater::BrownMushroomUpdater()
{
}

bool BrownMushroomUpdater::update(World &world, int x, int y, bool bg)
{
	plantUpdate(MYCELIUM, world, x, y, bg);
	return false;
}