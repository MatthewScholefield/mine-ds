#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include "../blockID.h"
#include "../blocks.h"
#include "ladder.h"
#include "../mobs/mobHandler.h"

LadderUpdater::LadderUpdater()
{
	blockID = LADDER;
	chance = NO_CHANCE;
}

void LadderUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	int &blockBelowXY = bg ? world->bgblocks[x][y + 1] : world->blocks[x][y + 1];
	if ((isBlockWalkThrough(blockBelowXY) && blockBelowXY != LADDER) && (bg || isBlockWalkThrough(world->bgblocks[x][y])))
	{
		int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
		blockXY = AIR;
		createItemMob(x, y, LADDER);
	}
}