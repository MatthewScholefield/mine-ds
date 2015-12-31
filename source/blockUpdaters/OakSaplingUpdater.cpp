#include "../world/WorldObject.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "OakSaplingUpdater.h"
#include "../trees.h"
#include "../world/worldRender.h"
#include "../mobs/mobHandler.h"

void saplingChanceUpdate(World &world, int x, int y, bool bg)
{
	if (world.brightness[x][y + 1] < 2)
	{
		short &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
		switch (blockXY)
		{
		case SAPLING_OAK:
			growOakTree(world, x, y);
			break;
		case SAPLING_JUNGLE:
			growJungleTree(world, x, y);
			break;
		case SAPLING_SPRUCE:
			growSpruceTree(world, x, y);
			break;
		}
		blockXY = AIR;
	}
}

void saplingUpdate(World &world, int x, int y, bool bg)
{
	short &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
	short blockBelowXY = bg ? world.bgblocks[x][y + 1] : world.blocks[x][y + 1];

	int requiredBlock = AIR;
	switch (blockXY)
	{
	case SAPLING_OAK:
		requiredBlock = GRASS;
		break;
	case SAPLING_JUNGLE:
		requiredBlock = GRASS_JUNGLE;
		break;
	case SAPLING_SPRUCE:
		requiredBlock = GRASS;
		break;
	}

	if (blockBelowXY != requiredBlock || isBlockWalkThrough(blockBelowXY))
	{
		createItemMob(x, y, blockXY, 1, leaf(blockXY));
		blockXY = AIR;
		return;
	}
}

OakSaplingUpdater::OakSaplingUpdater()
{
	chance = SAPLING_CHANCE_UPDATE;
}

bool OakSaplingUpdater::update(World &world, int x, int y, bool bg)
{
	saplingUpdate(world, x, y, bg);
	return false;
}

void OakSaplingUpdater::chanceUpdate(World &world, int x, int y, bool bg)
{
	saplingChanceUpdate(world, x, y, bg);
}