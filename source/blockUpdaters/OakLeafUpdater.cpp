#include "../world/WorldObject.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "../mobs/hurt.h"
#include "OakLeafUpdater.h"
#include "../mobs/mobHandler.h"

void leafChanceUpdateGeneric(WorldObject &world, int x, int y, bool bg)
{
	int leafID = bg ? world.bgblocks[x][y] : world.blocks[x][y];
	bool nearWood = false;
	for (int i = x - LEAF_SQUARE_RADIUS; i <= x + LEAF_SQUARE_RADIUS; ++i)
	{
		for (int j = y - LEAF_SQUARE_RADIUS; j <= y + LEAF_SQUARE_RADIUS; ++j)
			if (getType(bg ? world.bgblocks[i][j] : world.blocks[i][j]) == TYPE_WOOD)
			{
				nearWood = true;
				break;
			}
		if (nearWood)
			break;
	}
	if (!nearWood && bg)
		world.bgblocks[x][y] = AIR;
	else if (!nearWood)
		world.blocks[x][y] = AIR;
	if (!nearWood)
	{
		createItemMob(x, y, getSurvivalItem(leafID));
		updateAround(world,x,y);
	}
}

OakLeafUpdater::OakLeafUpdater()
{
	chance = LEAF_CHANCE_UPDATE;
}

void OakLeafUpdater::chanceUpdate(WorldObject &world, int x, int y, bool bg)
{
	leafChanceUpdateGeneric(world, x, y, bg);
}
