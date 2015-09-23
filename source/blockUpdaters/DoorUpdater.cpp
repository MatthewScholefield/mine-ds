#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "DoorUpdater.h"
#include "../mobs/hurt.h"
#include "../mobs/mobHandler.h"

DoorUpdater::DoorUpdater()
{
	chance = NO_CHANCE;
}

bool DoorUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	int &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
	int &blockBelowXY = bg ? world.bgblocks[x][y + 1] : world.blocks[x][y + 1];
	if (isBlockWalkThrough(blockBelowXY))
	{
		blockXY = AIR;
		createItemMob(x, y, DOOR_ITEM);
	}
	else
	{
		int &blockAboveXY = bg ? world.bgblocks[x][y - 1] : world.blocks[x][y - 1];
		blockXY = DOOR_OPEN_BOTTOM;
		blockAboveXY = DOOR_OPEN_TOP;
	}
	return false;
}

void DoorUpdater::chanceUpdate(WorldObject &world, int x, int y, bool bg) { }

DoorTopOpenUpdater::DoorTopOpenUpdater()
{
	chance = NO_CHANCE;
}

DoorTopClosedUpdater::DoorTopClosedUpdater()
{
	chance = NO_CHANCE;
}

DoorBottomOpenUpdater::DoorBottomOpenUpdater()
{
	chance = NO_CHANCE;
}

DoorBottomClosedUpdater::DoorBottomClosedUpdater()
{
	chance = NO_CHANCE;
}

void DoorTopUpdate(WorldObject &world, int x, int y, bool bg)
{
	int &blockBelowXY = bg ? world.bgblocks[x][y + 1] : world.blocks[x][y + 1];
	if (blockBelowXY == AIR)
	{
		int &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
		blockXY = AIR;
	}
}

void DoorBottomUpdate(WorldObject &world, int x, int y, bool bg)
{
	int &blockAboveXY = bg ? world.bgblocks[x][y - 1] : world.blocks[x][y - 1];
	if (blockAboveXY == AIR)
	{
		int &blockXY = bg ? world.bgblocks[x][y] : world.blocks[x][y];
		blockXY = AIR;
	}
}

bool DoorTopOpenUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	DoorTopUpdate(world, x, y, bg);
	return false;
}

bool DoorTopClosedUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	DoorTopUpdate(world, x, y, bg);
	return false;
}

bool DoorBottomOpenUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	DoorBottomUpdate(world, x, y, bg);
	return false;
}

bool DoorBottomClosedUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	DoorBottomUpdate(world, x, y, bg);
	return false;
}
