#include "../world/World.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "SnowGrassUpdater.h"

SnowGrassUpdater::SnowGrassUpdater()
{
	chance = SOIL_CHANCE_UPDATE;
}

bool SnowGrassUpdater::update(World &world, int x, int y, bool bg)
{
	if (world.blocks[x][y - 1] != SNOW_TOP)
	{
		short &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
		blockXY = bg && world.bgblocks[x][y - 1] == SNOW_TOP ? SNOW_GRASS : GRASS;
	}
	if (!bg && rand() % 2 == 0 && (x ^ y) % 7 == 0 && isBlockWalkThrough(world.blocks[x][y - 1]) && (world.bgblocks[x][y - 1] == AIR || world.bgblocks[x][y - 1] == SNOW_TOP)) {
		world.bgblocks[x][y - 1] = PUMPKIN;
	}
	return false;
}
