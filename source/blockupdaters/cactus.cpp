#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "cactus.h"
#include "../mobs/hurt.h"
#include "../mainGame.h"

cactusUpdater::cactusUpdater()
{
	blockID = CACTUS;
}

void cactusUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	int &blockBelowXY = bg ? world->bgblocks[x][y + 1] : world->blocks[x][y + 1];
	if (blockBelowXY != SAND && blockBelowXY != CACTUS)
	{
		blockXY = AIR;
		createItemMob(x, y, CACTUS);
	}
}
