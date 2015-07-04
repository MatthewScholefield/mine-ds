#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "oakSapling.h"
#include "../trees.h"
#include "../worldRender.h"
#include "../mainGame.h"

void saplingChanceUpdate(WorldObject *world, int x, int y, bool bg)
{
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];

	if (getBrightness(world, x, y + 1) < 2)
	{
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

void saplingUpdate(WorldObject *world, int x, int y, bool bg)
{
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	int blockBelowXY = bg ? world->bgblocks[x][y + 1] : world->blocks[x][y + 1];

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
	blockID = SAPLING_OAK;
	chance = SAPLING_CHANCE_UPDATE;
}

void OakSaplingUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	saplingUpdate(world, x, y, bg);
}

void OakSaplingUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg)
{
	saplingChanceUpdate(world, x, y, bg);
}