#include "mining.h" //for NUM_BLOCKS
#include "graphics/graphics.h"
#include "graphics/inventoryGraphics.h"
#include "titlescreen.h"
#include "inventory.h"
#include "blockID.h"
#include "message.h"
#include "titlescreen.h" // for isSurvival
#include <nds.h> // for keysDown()
#include "crafting.h"
recipe craftingRecipes[128];
int nextCraftingRecipe = 0;
void craftingInit()
{
	addCraftingRecipe(PLANKS,4,LOG,1);
}

void addCraftingRecipe(int block,int amount,int aid,int aamount,int bid,int bamount,int cid,int camount,int did,int damount)
{
	craftingRecipes[nextCraftingRecipe].createdblock.blockId = block;
	craftingRecipes[nextCraftingRecipe].createdblock.blockAmount = amount;
	craftingRecipes[nextCraftingRecipe].neededblocks[0].blockId = aid;
	craftingRecipes[nextCraftingRecipe].neededblocks[0].blockAmount = aamount;
	craftingRecipes[nextCraftingRecipe].neededblocks[1].blockId = bid;
	craftingRecipes[nextCraftingRecipe].neededblocks[1].blockAmount = bamount;
	craftingRecipes[nextCraftingRecipe].neededblocks[2].blockId = cid;
	craftingRecipes[nextCraftingRecipe].neededblocks[2].blockAmount = camount;
	craftingRecipes[nextCraftingRecipe].neededblocks[3].blockId = did;
	craftingRecipes[nextCraftingRecipe].neededblocks[3].blockAmount = damount;
	nextCraftingRecipe++;
}
void addCraftingRecipe(int block,int amount,int aid,int aamount,int bid,int bamount,int cid,int camount)
{
	addCraftingRecipe(block,amount,aid,aamount,bid,bamount,cid,camount,AIR,0);
}
void addCraftingRecipe(int block,int amount,int aid,int aamount,int bid,int bamount)
{
	//Kill the stack :D
	addCraftingRecipe(block,amount,aid,aamount,bid,bamount,AIR,0);
}
void addCraftingRecipe(int block,int amount,int aid,int aamount)
{
	//Kill the stack :D
	addCraftingRecipe(block,amount,aid,aamount,AIR,0);
}
void craftingMenuInit()
{
	disableInvGraphics();
	
}
