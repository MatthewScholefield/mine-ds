#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "grass.h"

grassUpdater::grassUpdater()
{
	blockID = GRASS;
	chance = SOIL_CHANCE_UPDATE;
}

void grassUpdater::chanceUpdate(worldObject* world, int x, int y, bool bg)
{
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	int &blockAboveXY = bg ? world->bgblocks[x][y - 1] : world->blocks[x][y - 1];
	if (!isBlockWalkThrough(blockAboveXY))
		blockXY = DIRT;
	else if (rand() % 10 == 0 && !bg && world->biome[x] == BIOME_SNOW)
	{
		blockXY = SNOW_GRASS;
		blockAboveXY = SNOW_TOP;
	}
	else if (rand() % 10 == 0 && bg && world->biome[x] == BIOME_SNOW && world->blocks[x][y - 1] == SNOW_TOP)
		blockXY = SNOW_GRASS;
}