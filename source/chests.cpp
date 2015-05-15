#include "world.h"
#include "general.h"
#include "blockID.h"
#include "inventory.h"

void createChest(WorldObject *world, int x, int y, bool bg)
{
	//Find first open chest slot
	int chestID = -1;
	for (int i = 0; i < MAX_CHESTS; ++i)
		if (!world->chestInUse[i])
			chestID = i;
	if (chestID == -1)
	{
		printLocalMessage("No more chests available");
		addInventory(CHEST);
		return;
	}
	if (bg)
	{
		world->bgblocks[x][y] = CHEST;
		world->data[x][y] &= 0x0000FFFF; //Clears left 8 bits
		chestID *= 0x00010000; //Shift left 8 bits
		world->data[x][y] |= chestID;
	}
	else
	{
		world->blocks[x][y] = CHEST;
		world->data[x][y] &= 0xFFFF0000; //Clears right 8 bits
		world->data[x][y] |= chestID;
	}
	world->chestInUse[chestID] = true;
}

int getChestID(WorldObject *world, int x, int y, bool bg)
{
	int databyte = world->data[x][y];
	if (bg)
	{
		databyte &= 0xFFFF0000;
		databyte /= 0x00010000;
	}
	else
		databyte &= 0x0000FFFF;
	return databyte;
}