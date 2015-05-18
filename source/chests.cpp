#include "world.h"
#include "general.h"
#include "blockID.h"
#include "inventory.h"
#include "mainGame.h"

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
		else
		{
			char buffer[20];
			sprintf(buffer, "Chest In Use: %d", i);
			printLocalMessage(buffer);
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
		world->data[x][y] &= 0x0000FFFF; //Clears left 16 bits
		chestID >>= 16; //Shift left 8 bits
		world->data[x][y] |= chestID;
	}
	else
	{
		world->blocks[x][y] = CHEST;
		world->data[x][y] &= 0xFFFF0000; //Clears right 16 bits
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
		databyte <<= 16;
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
	}
	else
	{
		if (world->blocks[x][y] != CHEST)
			return;
		blockID = world->data[x][y] & 0x0000FFFF;
	}
	world->chestInUse[blockID] = false;
	for (int i = 0; i < MAX_CHESTS; ++i)
	{
		if (world->chestInUse[i])
		{
			for (int j = 0; j < CHEST_SLOTS; ++j)
			{
				createItemMob(x, y, world->chests[i][j][INDEX_BLOCK_ID], world->chests[i][j][INDEX_AMOUNT], world->chests[i][j][INDEX_BLOCK_ID], (rand() % 25) / 10.0);
				world->chests[i][j][INDEX_BLOCK_ID] = world->chests[i][j][INDEX_AMOUNT] = 0;
			}
		}
	}
}