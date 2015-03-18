#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "plants.h"
#include "../mobs/hurt.h"
#include "../mainGame.h"

void plantUpdate(int beneathID, worldObject* world, int x, int y, bool bg)
{
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	int &blockBelowXY = bg ? world->bgblocks[x][y + 1] : world->blocks[x][y + 1];
	if (blockBelowXY != beneathID)
	{
		createItemMob(x, y, genericBlock(blockXY));
		blockXY = AIR;
	}
}

shrubUpdater::shrubUpdater()
{
	blockID = SHRUB;
}

void shrubUpdater::update(worldObject* world, int x, int y, bool bg)
{
	plantUpdate(SAND, world, x, y, bg);
}

tallGrassUpdater::tallGrassUpdater()
{
	blockID = TALL_GRASS;
}

void tallGrassUpdater::update(worldObject* world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
}

redFlowerUpdater::redFlowerUpdater()
{
	blockID = FLOWER_RED;
}

void redFlowerUpdater::update(worldObject* world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
}

yellowFlowerUpdater::yellowFlowerUpdater()
{
	blockID = FLOWER_YELLOW;
}

void yellowFlowerUpdater::update(worldObject* world, int x, int y, bool bg)
{
	plantUpdate(GRASS, world, x, y, bg);
}

redMushroomUpdater::redMushroomUpdater()
{
	blockID = MUSHROOM_RED;
}

void redMushroomUpdater::update(worldObject* world, int x, int y, bool bg)
{
	plantUpdate(MYCELIUM, world, x, y, bg);
}

brownMushroomUpdater::brownMushroomUpdater()
{
	blockID = MUSHROOM_BROWN;
}

void brownMushroomUpdater::update(worldObject* world, int x, int y, bool bg)
{
	plantUpdate(MYCELIUM, world, x, y, bg);
}