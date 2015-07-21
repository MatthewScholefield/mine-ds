#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "plants.h"
#include "../mobs/hurt.h"
#include "../mobs/mobHandler.h"

void plantUpdate(int beneathID, WorldObject* world, int x, int y, bool bg)
{
	int &blockBelowXY = bg ? world->bgblocks[x][y + 1] : world->blocks[x][y + 1];
	if (blockBelowXY != beneathID)
	{
		int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
		createItemMob(x, y, getSurvivalItem(blockXY));
		blockXY = AIR;
	}
}

ShrubUpdater::ShrubUpdater()
{
	blockID = SHRUB;
}

void ShrubUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	plantUpdate(SAND, world, x, y, bg);
}

TallGrassUpdater::TallGrassUpdater()
{
	blockID = TALL_GRASS;
}

void TallGrassUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
}

RedFlowerUpdater::RedFlowerUpdater()
{
	blockID = FLOWER_RED;
}

void RedFlowerUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
}

YellowFlowerUpdater::YellowFlowerUpdater()
{
	blockID = FLOWER_YELLOW;
}

void YellowFlowerUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
}

RedMushroomUpdater::RedMushroomUpdater()
{
	blockID = MUSHROOM_RED;
}

void RedMushroomUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	plantUpdate(MYCELIUM, world, x, y, bg);
}

BrownMushroomUpdater::BrownMushroomUpdater()
{
	blockID = MUSHROOM_BROWN;
}

void BrownMushroomUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	plantUpdate(MYCELIUM, world, x, y, bg);
}