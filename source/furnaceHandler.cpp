#include "world.h"
#include "blockID.h"
#include "mobs/mobHandler.h"
#include "inventory.h"
#include "general.h"
#include "interfaces/interfaceHandler.h"

int furnaceID = -1;

int getFurnaceID(WorldObject *world, int x, int y, bool bg)
{
	return (world->data[x][y] & (bg ? 0x000F0000 : 0x0000000F)) >> (bg ? 4 * 4 : 0);
}

void createFurnace(WorldObject *world, int x, int y, bool bg)
{
	int furnaceID = -1;
	for (int i = 0; i < MAX_FURNACES; ++i)
		if (!world->furnaces[i])
		{
			furnaceID = i;
			break;
		}
	if (furnaceID < 0)
	{
		printLocalMessage("No more furnaces available");
		addInventory(FURNACE);
		return;
	}
	world->furnaces[furnaceID] = new Furnace();
	if (bg)
	{
		world->bgblocks[x][y] = FURNACE;
		world->data[x][y] = (world->data[x][y] & 0x0000FFFF) | (furnaceID << 4 * 4);
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
	int id = getFurnaceID(world, x, y, bg);
	if (id < 0)
	{
		showError("Destroying non-existent furnace");
		return;
	}
	disperseItems(x, y, world->furnaces[id]->sourceBlock);
	disperseItems(x, y, world->furnaces[id]->fuelBlock);
	disperseItems(x, y, world->furnaces[id]->resultBlock);
	delete world->furnaces[id];
	world->furnaces[id] = nullptr;
	if (bg)
		world->bgblocks[x][y] = AIR;
	else
		world->blocks[x][y] = AIR;
}

void openFurnace(WorldObject *world, int x, int y, bool bg)
{
	if (furnaceID >= 0) //Another furnace is already opened
		return;
	furnaceID = getFurnaceID(world, x, y, bg);
	setInterface(INTERFACE_FURNACE);
}

void closeFurnace()
{
	furnaceID = -1;
}

int getOpenedFurnaceID()
{
	return furnaceID;
}