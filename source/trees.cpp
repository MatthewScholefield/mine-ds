#include <stdio.h>
#include <nds.h>
#include "world.h"
#include "blocks.h"
#include "blockID.h"

void leafVirus(WorldObject &world, int x, int y, int amount, bool down, int blockID)
{
	if (amount <= 0 || (unsigned) x >= WORLD_WIDTH)
		return;
	if (world.bgblocks[x][y] == AIR)
		world.bgblocks[x][y] = blockID;
	leafVirus(world, x + 1, y, amount - 1, down, blockID);
	leafVirus(world, x - 1, y, amount - 1, down, blockID);
	leafVirus(world, x, y - 1, amount - 1, down, blockID);
	if (down) leafVirus(world, x, y + 1, amount - 1, down, blockID);

}

void growTree(WorldObject &world, int x, int y, bool birch)
{
	int blockID = birch == true ? LOG_BIRCH : LOG_OAK;
	int height = 2 + rand() % 2;
	int i;
	for (i = y; i >= y - height; i--)
		world.bgblocks[x][i] = (world.bgblocks[x][i] == AIR ? blockID : world.bgblocks[x][i]); // Draw the trunk.
	//Now for the leaves...
	leafVirus(world, x, i, 2 + (rand() % 2), false, LEAVES_OAK);
}

void growOakTree(WorldObject &world, int x, int y)
{
	growTree(world, x, y, false);
}

void growBirchTree(WorldObject &world, int x, int y)
{
	growTree(world, x, y, true);
}

void growSpruceTree(WorldObject &world, int x, int y)
{
	int height = 2 + rand() % 2;
	int blockID = LOG_SPRUCE;
	int i;
	for (i = y; i >= y - height; i--)
	{
		world.bgblocks[x][i] = blockID; // Draw the trunk.
	}
	leafVirus(world, x, y - height, 2, false, LEAVES_SPRUCE);
	y = i;
	for (i = y; i >= y - height; i--)
	{
		world.bgblocks[x][i] = blockID; // Draw the trunk.
	}
	leafVirus(world, x, i, 2 + rand() % 2, false, LEAVES_SPRUCE);
}

void growJungleTree(WorldObject &world, int x, int y)
{
	int height = 2 + rand() % 6;
	int blockID = LOG_JUNGLE;
	int i;
	for (i = y; i >= y - height; i--)
	{
		world.bgblocks[x][i] = blockID; // Draw the trunk.
		world.bgblocks[x + 1][i] = blockID;
	}
	int random = rand() % 2;
	leafVirus(world, x + random, y - height, 3, false, LEAVES_JUNGLE);
	y = i;
	for (i = y; i >= y - height; i--)
	{
		world.bgblocks[x][i] = blockID; // Draw the trunk.
		world.bgblocks[x + 1][i] = blockID;
	}
	leafVirus(world, x, i, 2 + rand() % 3, false, LEAVES_JUNGLE);
	leafVirus(world, x + 1, i, 2 + rand() % 3, false, LEAVES_JUNGLE);
}

void growGiantMushroom(WorldObject &world, int x, int y)
{
	int height = 2 + rand() % 2;
	int i;
	for (i = y; i >= y - height; i--)
	{
		world.bgblocks[x][i] = MUSHROOM_STEM; // Draw the trunk.
	}
	//Now for the leaves...
	leafVirus(world, x, i, 2 + (rand() % 2), false, MUSHROOM_TOP);
}

void growCactus(WorldObject &world, int x, int y)
{
	int height = rand() % 3;
	int j;
	for (j = y - height; j <= y; ++j) world.bgblocks[x][j] = CACTUS;
}

void growFlower(WorldObject &world, int x, int y)
{
	if (rand() % 2) world.blocks[x][y] = RED_FLOWER;
	else world.blocks[x][y] = YELLOW_FLOWER;
}

void growPumpkinPatch(WorldObject &world, int x, int y)
{
	int i;
	for (i = x - 3 + rand() % 2; i <= x; ++i)
	{
		y = findFirstBlock(world, i);
		world.bgblocks[i][y - 1] = PUMPKIN;
	}
}
