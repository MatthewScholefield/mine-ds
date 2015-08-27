#include "world.h"
#include "general.h"
#include "blockID.h"
#include "inventory.h"
#include "mainGame.h"
#include "mobs/mobHandler.h"
#include "communications.h"

void createChest(WorldObject *world, int x, int y, bool bg)
{
	//Find first open chest slot
	int chestID = -1;
	for (int i = 0; i < MAX_CHESTS; ++i)
		if (!world->chestInUse[i])
		{
			chestID = i;
			break;
		}
	if (chestID == -1)
	{
		printLocalMessage("No more chests available");
		addInventory(CHEST);
		return;
	}
	if (bg)
	{
		world->bgblocks[x][y] = CHEST;
		world->data[x][y] &= 0x0000FFFF; //Clears higher 16 bits
		chestID <<= 16; //Shift higher 16 bits
		world->data[x][y] |= chestID;
	}
	else
	{
		world->blocks[x][y] = CHEST;
		world->data[x][y] &= 0xFFFF0000; //Clears lower 16 bits
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
		databyte >>= 16; //Shift right 16 bits
	}
	else
		databyte &= 0x0000FFFF;
	return databyte;
}

void destroyChest(WorldObject *world, int x, int y, bool bg)
{
	int blockID;
	if (bg)
	{
		if (world->bgblocks[x][y] != CHEST)
			return;
		blockID = world->data[x][y] & 0xFFFF0000;
		blockID >>= 16;
		world->bgblocks[x][y] = AIR;
	}
	else
	{
		if (world->blocks[x][y] != CHEST)
			return;
		blockID = world->data[x][y] & 0x0000FFFF;
		world->blocks[x][y] = AIR;
	}
	for (int j = 0; j < CHEST_SLOTS; ++j)
	{
		if (isSurvival())
		{
			for (int i = 0; i < 3; ++i)
				createItemMob(x, y, world->chests[blockID].blocks[j].blockId, world->chests[blockID].blocks[j].blockAmount / 4, world->chests[blockID].blocks[j].blockId, ((double(rand() % 25)) / 40.0) * (rand() % 2 == 0 ? -1.0 : 1.0));
			createItemMob(x, y, world->chests[blockID].blocks[j].blockId, world->chests[blockID].blocks[j].blockAmount - 3 * (world->chests[blockID].blocks[j].blockAmount / 4), world->chests[blockID].blocks[j].blockId, ((double(rand() % 25)) / 40.0) * (rand() % 2 == 0 ? -1.0 : 1.0));
		}
		world->chests[blockID].blocks[j].blockId = world->chests[blockID].blocks[j].blockAmount = 0;
	}
	if (isSurvival())
		createItemMob(x, y, CHEST);
	world->chestInUse[blockID] = false;
}
