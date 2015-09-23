#include "../world.h"
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

bool SnowGrassUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	if (world.blocks[x][y - 1] != SNOW_TOP)
	{
		int &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
		blockXY = bg && world.bgblocks[x][y - 1] == SNOW_TOP ? SNOW_GRASS : GRASS;
	}
	return false;
}
