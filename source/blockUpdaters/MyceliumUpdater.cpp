#include "../world/World.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "MyceliumUpdater.h"

MyceliumUpdater::MyceliumUpdater()
{
	chance = SOIL_CHANCE_UPDATE;
}

void MyceliumUpdater::chanceUpdate(World &world, int x, int y, bool bg)
{
	if (!bg && (x ^ y) % 7 == 2 && rand() % 2 == 0 && (world.blocks[x][y - 1] == AIR || world.blocks[x][y - 1] == TALL_GRASS)) {
		world.blocks[x][y - 1] = rand() % 2 ? MUSHROOM_RED : MUSHROOM_BROWN;
	}
	if (!bg)
	{
		if (!isBlockWalkThrough(world.blocks[x][y - 1]))
		{
			world.blocks[x][y] = DIRT;
		}
	}
	else
	{
		if (!isBlockWalkThrough(world.blocks[x][y - 1]) || !isBlockWalkThrough(world.bgblocks[x][y - 1]))
		{
			world.bgblocks[x][y] = DIRT;
		}
	}
}
