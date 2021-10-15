#include "../world/World.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "GrassUpdater.h"
#include "../dayNight.h"
#include "../world/worldRender.h"

GrassUpdater::GrassUpdater()
{
	chance = SOIL_CHANCE_UPDATE;
}

void GrassUpdater::chanceUpdate(World &world, int x, int y, bool bg)
{
	short &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
	short &blockAboveXY = bg ? world.bgblocks[x][y - 1] : world.blocks[x][y - 1];
	if (!isBlockWalkThrough(blockAboveXY) || (world.brightness[x][y] <= 13 && isDay(world)))
		blockXY = DIRT;
	else if (world.biome[x] == Biome::SNOW && !bg && rand() % 10 == 0 && blockAboveXY == AIR)
	{
		blockXY = SNOW_GRASS;
		blockAboveXY = SNOW_TOP;
	}
	else if (world.biome[x] == Biome::SNOW && bg && rand() % 10 == 0 && world.blocks[x][y - 1] == SNOW_TOP)
		blockXY = SNOW_GRASS;
	if (!bg && rand() % 2 == 0 && (x ^ y) % 7 == 0 && (blockAboveXY == AIR || blockAboveXY == TALL_GRASS)) {
		world.blocks[x][y - 1] = rand() % 2 ? FLOWER_RED : FLOWER_YELLOW;
	}
}
