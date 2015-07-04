#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "junglegrass.h"

JungleGrassUpdater::JungleGrassUpdater()
{
	blockID = GRASS_JUNGLE;
	chance = SOIL_CHANCE_UPDATE;
}

void JungleGrassUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg)
{
	if (!bg)
	{
		if (!isBlockWalkThrough(world->blocks[x][y - 1]))
		{
			world->blocks[x][y] = DIRT;
		}
	}
	else
	{
		if (!isBlockWalkThrough(world->blocks[x][y - 1]) || !isBlockWalkThrough(world->bgblocks[x][y - 1]))
		{
			world->bgblocks[x][y] = DIRT;
		}
	}
}
