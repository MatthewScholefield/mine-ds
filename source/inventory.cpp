#include "inventory.h"
#include "blockID.h"
#include "general.h"
#include "mobs/mobHandler.h"
#include "mainGame.h"
#include "graphics/interfaces/interfaceHandler.h"

Inventory mainPlayerInv;

Inventory &getInventoryRef()
{
	return mainPlayerInv;
}

static int findFreeSlot(int blockID)
{
	int space = -1;
	for (int i = 0; i < NUM_INV_SPACES; ++i)
		if (mainPlayerInv.blocks[i].ID == blockID && mainPlayerInv.blocks[i].amount < 64)
		{ //Merge
			space = i;
			break;
		}
		else if (mainPlayerInv.blocks[i].ID == AIR)
			space = i; //Last resort
	return space;
}

bool addInventory(int blockID, int amount)
{
	if (blockID == AIR)
		return false;
	int space = findFreeSlot(blockID);
	if (space < 0)
	{
		printLocalMessage("No space for item");
		return false;
	}
	mainPlayerInv.blocks[space].ID = blockID;
	if (isSurvival())
		mainPlayerInv.blocks[space].amount += amount;
	else
		mainPlayerInv.blocks[space].amount = 1;
	Interface::triggerUpdate();
	return true;
}

bool subInventory(int blockID, int amount)
{
	if (isSurvival() == false) //Always return true in creative!
		return true;

	for (auto &i : mainPlayerInv.blocks)
	{
		if (i.ID == blockID)
		{
			i.amount -= amount;
			if (i.amount < 0)
			{
				i.amount += amount;
				return false;
			}
			else if (i.amount == 0)
				i.ID = AIR;
			Interface::triggerUpdate();
			return true;
		}
	}
	return false;
}

static int getInventorySlot(int blockID) //returns slot of blockid in inventory
{
	int space = -1;
	for (int i = 0; i < NUM_INV_SPACES; ++i)
	{
		//Found the correct block with correct id.
		if (mainPlayerInv.blocks[i].ID == blockID)
		{
			space = i;
			break;
		}
	}
	return space;
}

int checkInventory(int blockID) //returns quantity of blockid in inventory
{
	if (blockID == AIR)
		return 64;
	int space = getInventorySlot(blockID);
	if (space < 0)
		return 0;
	return mainPlayerInv.blocks[space].amount;
}

int getHandID()
{
	return mainPlayerInv.hand < 0 ? AIR : mainPlayerInv.blocks[mainPlayerInv.hand].ID;
}

void clearInventory() //clears inventory
{
	int i;
	for (i = 0; i < NUM_INV_SPACES; ++i)
	{
		mainPlayerInv.blocks[i].amount = 0;
		mainPlayerInv.blocks[i].ID = 0;
	}
}

void saveInventory(FILE* data)
{
	for (auto &i : mainPlayerInv.blocks)
		i.saveToFile(data);
}

void loadInventory(FILE* data)
{
	clearInventory();
	for (auto &i : mainPlayerInv.blocks)
		i.loadFromFile(data);
}

void spillInvItems(int x, int y)
{
	for (int j = 0; j < NUM_INV_SPACES; ++j)
	{
		for (int i = 0; i < 3; ++i)
			createItemMob(x / 16, (y - 24) / 16, mainPlayerInv.blocks[j].ID,
						mainPlayerInv.blocks[j].amount / 4, mainPlayerInv.blocks[j].ID,
						((double(rand() % 25)) / 40.0) * (rand() % 2 == 0 ? -1.0 : 1.0));

		createItemMob(x / 16, (y - 24) / 16, mainPlayerInv.blocks[j].ID,
					mainPlayerInv.blocks[j].amount - 3 * (mainPlayerInv.blocks[j].amount / 4),
					mainPlayerInv.blocks[j].ID,
					((double(rand() % 25)) / 40.0) * (rand() % 2 == 0 ? -1.0 : 1.0));
	}
}