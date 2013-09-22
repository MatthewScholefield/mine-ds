/*
-----------------------------------------
|             Inventory.cpp             |
|           Inventory Funtions          |
-----------------------------------------
*/
#include "mining.h" //for NUM_BLOCKS
#include "graphics/graphics.h"
#include "graphics/inventoryGraphics.h"
#include "titlescreen.h"
#include "inventory.h"
#include "blockID.h"
#include "message.h"
#include "titlescreen.h" // for isSurvival
#include <nds.h> // for keysDown()
bool loadedGraphic = false;
Graphic heldBlock;
/*					A reminder:



INVENTORY STRUCT USES blockId, not blockID, for what ever reason!!!!!
*/


int showingInventory;
Inventory mainPlayerInv;
bool addInventory(int blockID, int amount) //adds the specified amount to a blockvalue
{
	//First find a spot availabe for the block id
	if (isSurvival()==false) //Always return true in creative!
		return true;
	int i;
	int space=-1;
	if (blockID == GRASS || blockID == JUNGLE_GRASS || blockID == SNOW_GRASS)
		blockID = DIRT;
	if (blockID == BEDROCK)
		return false; //Will not destroy block
	if (blockID==SNOW_TOP)
		return true; //Will still destory block (it just wont be added to the inventory)
	if (blockID==COAL_ORE && (getSelectedblock()==PICKAXE_WOOD || getSelectedblock()==PICKAXE_STONE || getSelectedblock()==PICKAXE_IRON || getSelectedblock()==PICKAXE_GOLD || getSelectedblock()==PICKAXE_DIAMOND))
		blockID=COAL;
	else if (blockID==COAL_ORE)
		return true;
	if (blockID==IRON_ORE && (getSelectedblock()==PICKAXE_STONE || getSelectedblock()==PICKAXE_IRON || getSelectedblock()==PICKAXE_DIAMOND))
		blockID=IRON;
	else if (blockID==IRON_ORE)
		return true;
	if (blockID==GOLD_ORE && (getSelectedblock()==PICKAXE_IRON || getSelectedblock()==PICKAXE_DIAMOND))
		blockID=GOLD;
	else if (blockID==GOLD_ORE)
		return true;
	if (blockID==DIAMOND_ORE && (getSelectedblock()==PICKAXE_IRON || getSelectedblock()==PICKAXE_DIAMOND))
		blockID=DIAMOND;
	else if (blockID==DIAMOND_ORE)
		return true;

	if (blockID==AIR) return false;
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
		return false; //We still want the block to be broken - Do we ?
	}
	//Space now equals the block for the id...
	mainPlayerInv.blocks[space].blockId=blockID;
	mainPlayerInv.blocks[space].blockAmount+=amount;
	updateInvGraphics();
	return true;
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
int getBlockID(int invSlot)
{
	return mainPlayerInv.blocks[invSlot].blockId;
}
int getBlockAmount(int invSlot)
{
	return mainPlayerInv.blocks[invSlot].blockAmount;
}
void clearInventory () //clears inventory
{
	int i;
	for (i=0;i<NUM_INV_SPACES;i++)
	{
		mainPlayerInv.blocks[i].blockAmount=0;
		mainPlayerInv.blocks[i].blockId=0;
	}
	updateInvGraphics();
	showingInventory = 0;
}
void changeGraphic(int blockID)
{
	if (loadedGraphic == true)
	{
		unloadGraphic(&heldBlock);
		loadedGraphic = false;	
	}
	loadGraphicSub(&heldBlock,2,blockID);
	loadedGraphic = true;
}
void updateInventory(touchPosition* touch)
{
	if (!isSurvival())
		return;
	if (showingInventory == 0)
	{
		if (keysDown() & KEY_Y)
		{
			lcdMainOnTop();
			showingInventory = 1;
			show_message("In inventory\n");
			miningSetScene(true);
			drawBackground();
			consoleClear();
			changeGraphic(AIR);
		}
	}
	else if (showingInventory == 1)
	{
		showGraphic(&heldBlock,0,0,false,0);
		if (keysDown() & KEY_TOUCH)
		{
			if (touch->px > 8 && touch->py > 72 && touch->py < 120 && touch->px < 248) 
			{
				int space = -1;
				space+=16*((touch->py - 72)/24);
				space+=(touch->px - 8)/16;
				setSelectedBlock(mainPlayerInv.blocks[space].blockId);
				changeGraphic(mainPlayerInv.blocks[space].blockId);
			}
		}
		if (keysDown() & KEY_Y)
		{
			lcdMainOnBottom();
			showingInventory = 0;
			show_message("Left inventory\n");
			miningSetScene(false);
			drawBackground();
		}
	}
}
