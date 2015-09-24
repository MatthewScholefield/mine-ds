#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "GrassUpdater.h"
#include "../dayNight.h"
#include "../worldRender.h"

GrassUpdater::GrassUpdater()
{
	chance = SOIL_CHANCE_UPDATE;
}

void GrassUpdater::chanceUpdate(WorldObject &world, int x, int y, bool bg)
{
	int &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
	int &blockAboveXY = bg ? world.bgblocks[x][y - 1] : world.blocks[x][y - 1];
	if (!isBlockWalkThrough(blockAboveXY) || (getBrightness(world, x, y) >= 2 && isDay(world)))
		blockXY = DIRT;
	else if (world.biome[x] == BIOME_SNOW && !bg && rand() % 10 == 0 && blockAboveXY == AIR)
	{
		blockXY = SNOW_GRASS;
		blockAboveXY = SNOW_TOP;
	}
	else if (world.biome[x] == BIOME_SNOW && bg && rand() % 10 == 0 && world.blocks[x][y - 1] == SNOW_TOP)
		blockXY = SNOW_GRASS;
}