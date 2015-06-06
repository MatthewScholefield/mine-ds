#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "mycelium.h"

myceliumUpdater::myceliumUpdater()
{
	blockID = MYCELIUM;
	chance = SOIL_CHANCE_UPDATE;
}

void myceliumUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg)
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
