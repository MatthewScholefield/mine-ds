#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "snowTop.h"

SnowTopUpdater::SnowTopUpdater()
{
	blockID = SNOW_TOP;
}

void SnowTopUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	if (!bg && isBlockWalkThrough(world->blocks[x][y + 1]))
	{
		world->blocks[x][y] = AIR;
		if ((world->bgblocks[x][y + 1] == SNOW_GRASS || world->bgblocks[x][y + 1] == GRASS) && world->bgblocks[x][y] == AIR)
			world->bgblocks[x][y] = SNOW_TOP;
	}
	else if (bg && (!isBlockWalkThrough(world->blocks[x][y + 1]) || isBlockWalkThrough(world->bgblocks[x][y + 1])))
		world->bgblocks[x][y] = AIR;
}