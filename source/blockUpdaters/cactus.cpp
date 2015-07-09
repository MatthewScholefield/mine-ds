#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "cactus.h"
#include "../mobs/hurt.h"
#include "../mobs/mobHandler.h"

CactusUpdater::CactusUpdater()
{
	blockID = CACTUS;
	chance = CACTUS_CHANCE_UPDATE;
}

void CactusUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	int &blockBelowXY = bg ? world->bgblocks[x][y + 1] : world->blocks[x][y + 1];
	if (blockBelowXY != SAND && blockBelowXY != CACTUS)
	{
		blockXY = AIR;
		createItemMob(x, y, CACTUS);
	}
}

void CactusUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg)
{
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	int &blockAboveXY = bg ? world->bgblocks[x][y - 1] : world->blocks[x][y - 1];
	int &blockBelowXY = bg ? world->bgblocks[x][y + 1] : world->blocks[x][y + 1];
	if (blockAboveXY == AIR && blockBelowXY == SAND)
	{
		blockAboveXY = blockXY;
	}
}
