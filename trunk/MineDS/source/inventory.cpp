/*
-----------------------------------------
|             Inventory.cpp             |
|           Inventory Funtions          |
-----------------------------------------
*/
#include "mining.h" //for NUM_BLOCKS
#include "graphics/inventoryGraphics.h"
#include "titlescreen.h"
int inventory[NUM_BLOCKS];
void addInventory(int blockID, int amount) //adds the specified amount to a blockvalue
{
	inventory [blockID] += amount;
	updateInvGraphics();
}
void addInventory(int blockID)
{
	addInventory(blockID,1);
}
bool subInventory (int blockID, int amount) //subtracts the specified amount to a blockvalue
{
	if (isSurvival()==false) //Always return true in creative!
		return true;
	inventory [blockID] -= amount;
	if (inventory[blockID] < 0)
	{
		inventory[blockID]+=amount;
		return false;
	}
	updateInvGraphics();
	return true;
}
int checkInventory (int blockID) //returns quantity of blockid in inventory
{
	return inventory [blockID];
}
void clearInventory () //clears inventory
{
	int i;
	for (i = 0; i <= NUM_BLOCKS; i++)
		inventory [i] = 0;
	updateInvGraphics();
}
