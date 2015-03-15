#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "snowGrass.h"

snowGrassUpdater::snowGrassUpdater()
{
	blockID = SNOW_GRASS;
	chance = SOIL_CHANCE_UPDATE;
}

void snowGrassUpdater::update(worldObject* world, int x, int y, bool bg)
{
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	if (world->blocks[x][y - 1] != SNOW_TOP)
		blockXY = bg && world->bgblocks[x][y - 1] == SNOW_TOP ? SNOW_GRASS : GRASS;
}
