#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "leaf.h"

void leafChanceUpdateGeneric(worldObject* world, int x, int y, bool bg)
{
	bool nearWood = false;
	for (int i = x - LEAF_SQUARE_RADIUS; i < x + LEAF_SQUARE_RADIUS; ++i)
	{
		for (int j = y - LEAF_SQUARE_RADIUS; j < y + LEAF_SQUARE_RADIUS; ++j)
			if (getType(bg ? world->bgblocks[i][j] : world->blocks[i][j]) == WOOD)
			{
				nearWood = true;
				break;
			}
		if (nearWood)
			break;
	}
	if (!nearWood && bg)
		world->bgblocks[x][y] = AIR;
	else if (!nearWood)
		world->blocks[x][y] = AIR;
}

leafUpdater::leafUpdater()
{
	blockID = LEAF;
	chance = LEAF_CHANCE_UPDATE;
}

void leafUpdater::chanceUpdate(worldObject* world, int x, int y, bool bg)
{
	leafChanceUpdateGeneric(world, x, y, bg);
}