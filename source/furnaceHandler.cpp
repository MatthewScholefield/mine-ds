#include "world.h"
#include "blockID.h"
#include "mobs/mobHandler.h"
#include "inventory.h"
#include "general.h"
#include "interfaces/interfaceHandler.h"
#include "furnaceHandler.h"
#include "blocks.h"
#include "Recipe.h"
#define NUM_FURNACE_RECIPES 6

int furnaceID = -1;
static const Recipe furnaceRecipes[NUM_FURNACE_RECIPES] = {
	{GLASS, 1, SAND, 1},
	{BEEF_COOKED, 1, BEEF_RAW, 1},
	{CHICKEN_COOKED, 1, CHICKEN_RAW, 1},
	{PORKCHOP_COOKED, 1, PORKCHOP_RAW, 1},
	{DIAMOND, 1, DIAMOND_ORE, 1},
	{INGOT_IRON, 1, IRON_ORE, 1}
};

void convertItemToFuel(Furnace &furnace)
{
	int newFuel = fuelAmount(furnace.fuelBlock.blockId);
	if (newFuel > 0 && furnace.fuel < 1)
	{
		furnace.fuel = newFuel;
		if (--furnace.fuelBlock.blockAmount < 1)
			furnace.fuelBlock.blockId = AIR;
	}
	else
		printXY(1, rand() % 10, furnace.fuelBlock.blockId);
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