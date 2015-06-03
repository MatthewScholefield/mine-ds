#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "../mobs/hurt.h"
#include "leaf.h"
#include "../mainGame.h"

void leafChanceUpdateGeneric(WorldObject* world, int x, int y, bool bg)
{
	int leafID = bg ? world->bgblocks[x][y] : world->blocks[x][y];
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
	if (!nearWood)
		createItemMob(x, y, genericBlock(leafID));
}

leafUpdater::leafUpdater()
{
	blockID = LEAVES_OAK;
	chance = LEAF_CHANCE_UPDATE;
}

void leafUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg)
{
	leafChanceUpdateGeneric(world, x, y, bg);
}
