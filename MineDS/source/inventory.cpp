/*
-----------------------------------------
|             Inventory.cpp             |
|           Inventory Funtions          |
-----------------------------------------
*/
#include "mining.h" //for NUM_BLOCKS
#include "graphics/inventoryGraphics.h"
#include "titlescreen.h"
#include "inventory.h"
#include "blockID.h"
#include "message.h"
Inventory mainPlayerInv;
int inventory[NUM_BLOCKS];
bool addInventory(int blockID, int amount) //adds the specified amount to a blockvalue
{
	//First find a spot availabe for the block id
	int i;
	int space=-1;
	for (i=0;i<NUM_INV_SPACES;i++)
	{
		//Found the correct block with correct id.
		if (mainPlayerInv.blocks[i].blockId==blockID)
		{
			space=i;
			i=NUM_INV_SPACES+1;
			//Skip other inventory spaces...
		}
		//Found a sutiable air space.
		else if (mainPlayerInv.blocks[i].blockId==AIR)
		{
			//Set the space as last resort, try to find correct inventory space.
			space=i;
		}
	}	
	if (space==-1)
	{
		show_message("No space for item");
		return false;
	}
	//Space now equals the block for the id...
	mainPlayerInv.blocks[space].blockId=blockID;
	mainPlayerInv.blocks[space].blockAmount+=amount;
	inventory [blockID] += amount;
	updateInvGraphics();
	return false;
}
void addInventory(int blockID)
{
	addInventory(blockID,1);
}
bool subInventory (int blockID, int amount) //subtracts the specified amount to a blockvalue
{
	if (isSurvival()==false) //Always return true in creative!
		return true;
	int i;
	int space=-1;
	for (i=0;i<NUM_INV_SPACES;i++)
	{
		//Found the correct block with correct id.
		if (mainPlayerInv.blocks[i].blockId==blockID)
		{
			space=i;
			i=NUM_INV_SPACES+1;
			//Skip other inventory spaces...
		}
	}	
	if (space==-1)
	{
		show_message("No item to remove\n");
		return false;
	}
	mainPlayerInv.blocks[space].blockAmount-=amount;
	if (mainPlayerInv.blocks[space].blockAmount<0)
	{
		mainPlayerInv.blocks[space].blockAmount+=amount;
		return false;
	}
	else if (mainPlayerInv.blocks[space].blockAmount==0)
	{
		mainPlayerInv.blocks[space].blockId=AIR;
	}
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
	int i;
	int space=-1;
	for (i=0;i<NUM_INV_SPACES;i++)
	{
		//Found the correct block with correct id.
		if (mainPlayerInv.blocks[i].blockId==blockID)
		{
			space=i;
			i=NUM_INV_SPACES+1;
			//Skip other inventory spaces...
		}
	}	
	if (space==-1)
	{
		return 0;
	}
	if (blockID==AIR)
		return 64;
	return mainPlayerInv.blocks[space].blockAmount;
}
void clearInventory () //clears inventory
{
	int i;
	for (i = 0; i <= NUM_BLOCKS; i++)
		inventory [i] = 0;
	updateInvGraphics();
}
