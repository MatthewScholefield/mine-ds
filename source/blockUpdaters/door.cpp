#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "door.h"
#include "../mobs/hurt.h"
#include "../mobs/mobHandler.h"

DoorUpdater::DoorUpdater()
{
	blockID = DOOR_ITEM;
	chance = NO_CHANCE;
}

void DoorUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	int &blockBelowXY = bg ? world->bgblocks[x][y + 1] : world->blocks[x][y + 1];
	if (isBlockWalkThrough(blockBelowXY))
	{
		blockXY = AIR;
		createItemMob(x, y, DOOR_ITEM);
	}
	else
	{
		int &blockAboveXY = bg ? world->bgblocks[x][y - 1] : world->blocks[x][y - 1];
		blockXY = DOOR_OPEN_BOTTOM;
		blockAboveXY = DOOR_OPEN_TOP;
	}
}

void DoorUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg) { }

DoorTopOpenUpdater::DoorTopOpenUpdater()
{
	blockID = DOOR_OPEN_TOP;
	chance = NO_CHANCE;
}

DoorTopClosedUpdater::DoorTopClosedUpdater()
{
	blockID = DOOR_CLOSED_TOP;
	chance = NO_CHANCE;
}

DoorBottomOpenUpdater::DoorBottomOpenUpdater()
{
	blockID = DOOR_OPEN_BOTTOM;
	chance = NO_CHANCE;
}

DoorBottomClosedUpdater::DoorBottomClosedUpdater()
{
	blockID = DOOR_CLOSED_BOTTOM;
	chance = NO_CHANCE;
}

void DoorTopUpdate(WorldObject* world, int x, int y, bool bg)
{
	int &blockBelowXY = bg ? world->bgblocks[x][y + 1] : world->blocks[x][y + 1];
	if (blockBelowXY == AIR)
	{
		int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
		blockXY = AIR;
	}
}

void DoorBottomUpdate(WorldObject* world, int x, int y, bool bg)
{
	int &blockAboveXY = bg ? world->bgblocks[x][y - 1] : world->blocks[x][y - 1];
	if (blockAboveXY == AIR)
	{
		int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
		blockXY = AIR;
	}
}

void DoorTopOpenUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	DoorTopUpdate(world, x, y, bg);
}

void DoorTopClosedUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	DoorTopUpdate(world, x, y, bg);
}

void DoorBottomOpenUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	DoorBottomUpdate(world, x, y, bg);
}

void DoorBottomClosedUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	DoorBottomUpdate(world, x, y, bg);
}
