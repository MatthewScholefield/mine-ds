#include "../WorldObject.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include "../blockID.h"
#include "../blocks.h"
#include "LadderUpdater.h"
#include "../mobs/mobHandler.h"

LadderUpdater::LadderUpdater()
{
	chance = NO_CHANCE;
}

bool LadderUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	short &blockBelowXY = bg ? world.bgblocks[x][y + 1] : world.blocks[x][y + 1];
	if ((isBlockWalkThrough(blockBelowXY) && blockBelowXY != LADDER) && (bg || isBlockWalkThrough(world.bgblocks[x][y])))
	{
		short &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
		blockXY = AIR;
		createItemMob(x, y, LADDER);
	}
	return false;
}