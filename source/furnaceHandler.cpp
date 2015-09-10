#include "world.h"
#include "blockID.h"
#include "mobs/mobHandler.h"
#include "inventory.h"
#include "general.h"
#include "interfaces/interfaceHandler.h"
#include "furnaceHandler.h"
#include "blocks.h"
#include "Recipe.h"
#define NUM_FURNACE_RECIPES 11

int furnaceID = -1;
static const FurnaceRecipe furnaceRecipes[NUM_FURNACE_RECIPES] = {
	{1, GLASS, SAND},
	{2, BEEF_COOKED, BEEF_RAW},
	{2, CHICKEN_COOKED, CHICKEN_RAW},
	{2, PORKCHOP_COOKED, PORKCHOP_RAW},
	{3, DIAMOND, DIAMOND_ORE},
	{2, INGOT_GOLD, GOLD_ORE},
	{2, INGOT_IRON, IRON_ORE},
	{4, COAL, LOG_BIRCH},
	{4, COAL, LOG_JUNGLE},
	{4, COAL, LOG_OAK},
	{4, COAL, LOG_SPRUCE}
};

void convertItemToFuel(Furnace &furnace)
{
	int newFuel = fuelAmount(furnace.fuelBlock.ID);
	if (newFuel > 0 && furnace.fuel < 1)
	{
		furnace.fuel = newFuel;
		if (--furnace.fuelBlock.amount < 1)
			furnace.fuelBlock.ID = AIR;
	}
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
	disperseItems(x, y, block.ID, block.amount);
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

int fuelNeeded(const Furnace &furnace)
{
	for (auto &i : furnaceRecipes)
		if (i.needed == furnace.sourceBlock.ID && (i.result == furnace.resultBlock.ID || furnace.resultBlock.ID == AIR))
			return i.fuel;
	return 0;
}

void createResult(Furnace &furnace)
{
	int id = 0;
	for (id = 0; id < NUM_FURNACE_RECIPES && furnaceRecipes[id].needed != furnace.sourceBlock.ID; ++id);
	if (id >= NUM_FURNACE_RECIPES || (furnace.resultBlock.ID != AIR && furnaceRecipes[id].result != furnace.resultBlock.ID))
		return;
	if (furnace.resultBlock.ID == AIR)
		furnace.resultBlock = InvBlock(furnaceRecipes[id].result, 1);
	else
		++furnace.resultBlock.amount;
	if (--furnace.sourceBlock.amount < 1)
		furnace.sourceBlock.ID = AIR;
}