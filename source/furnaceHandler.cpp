#include "world.h"
#include "blockID.h"
#include "mainGame.h"
#include "inventory.h"
#include "general.h"
#include <cstdlib>

void createFurnace(WorldObject *world, int x, int y, bool bg)
{
	int furnaceID = -1;
	for (int i = 0; i < MAX_FURNACES; ++i)
		if (!world->furnaces[i])
		{
			furnaceID = i;
			break;
		}
	if (furnaceID == -1)
	{
		printLocalMessage("No more furnaces available");
		addInventory(FURNACE);
		return;
	}
	world->furnaces[furnaceID] = new Furnace();
	if (bg)
	{
		world->bgblocks[x][y] = FURNACE;
		world->data[x][y] = (world->data[x][y] & 0x0000FFFF) | (furnaceID << 16);
	}
	else
	{
		world->blocks[x][y] = FURNACE;
		world->data[x][y] = (world->data[x][y] & 0xFFFF0000) | (furnaceID);
	}
}

void disperseItems(int x, int y, int blockID, int amount)
{
	for (int i = 0; i < 3; ++i)
		createItemMob(x, y, blockID, amount / 4, blockID, ((double(rand() % 25)) / 40.0) * (rand() % 2 == 0 ? -1.0 : 1.0));
	createItemMob(x, y, blockID, amount - 3 * (amount / 4), blockID, ((double(rand() % 25)) / 40.0) * (rand() % 2 == 0 ? -1.0 : 1.0));
}

void disperseItems(int x, int y, InvBlock block)
{
	disperseItems(x, y, block.blockId, block.blockAmount);
}

void destroyFurnace(WorldObject *world, int x, int y, bool bg)
{
	int id = (world->data[x][y] & (bg ? 0xFFFF0000 : 0x0000FFFF)) >> (bg ? 16 : 0);
	disperseItems(x, y, world->furnaces[id]->source);
	disperseItems(x, y, world->furnaces[id]->fuel);
	disperseItems(x, y, world->furnaces[id]->result);
	delete world->furnaces[id];
	world->furnaces[id] = NULL;
}