#include "../world/World.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "CactusUpdater.h"
#include "../mobs/hurt.h"
#include "../mobs/mobHandler.h"

CactusUpdater::CactusUpdater()
{
	chance = CACTUS_CHANCE_UPDATE;
}

bool CactusUpdater::update(World &world, int x, int y, bool bg)
{
	short &blockBelowXY = bg ? world.bgblocks[x][y + 1] : world.blocks[x][y + 1];
	if (blockBelowXY != SAND && blockBelowXY != CACTUS)
	{
		short &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
		blockXY = AIR;
		createItemMob(x, y, CACTUS);
		return true;
	}
	return false;
}

void CactusUpdater::chanceUpdate(World &world, int x, int y, bool bg)
{
	short &blockAboveXY = bg ? world.bgblocks[x][y - 1] : world.blocks[x][y - 1];
	short &blockBelowXY = bg ? world.bgblocks[x][y + 1] : world.blocks[x][y + 1];
	if (blockAboveXY == AIR && blockBelowXY == SAND)
	{
		short &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
		blockAboveXY = blockXY;
	}
}
