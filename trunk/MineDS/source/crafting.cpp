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
	addCraftingRecipe(STICK,4,PLANKS,2);
	addCraftingRecipe(PICKAXE_WOOD,1,PLANK,3,STICK,2);
	addCraftingRecipe(PICKAXE_STONE,1,COBBLESTONE,3,STICK,2);
	addCraftingRecipe(PICKAXE_IRON,1,INGOT_IRON,3,STICK,2);
	addCraftingRecipe(PICKAXE_DIAMOND,1,DIAMOND,3,STICK,2);
	addCraftingRecipe(AXE_WOOD,1,PLANK,3,STICK,2);
	addCraftingRecipe(AXE_STONE,1,COBBLESTONE,3,STICK,2);
	addCraftingRecipe(AXE_IRON,1,INGOT_IRON,3,STICK,2);
	addCraftingRecipe(AXE_DIAMOND,1,DIAMOND,3,STICK,2);
	addCraftingRecipe(SWORD_WOOD,1,PLANK,2,STICK,1);
	addCraftingRecipe(SWORD_STONE,1,COBBLESTONE,2,STICK,1);
	addCraftingRecipe(SWORD_IRON,1,INGOT_IRON,2,STICK,1);
	addCraftingRecipe(SWORD_DIAMOND,1,DIAMOND,2,STICK,1);
	addCraftingRecipe(TORCH,4,COAL,1,STICK,1);
	addCraftingRecipe(CRAFTING_TABLE,1,PLANKS,4);

	//Furnace Recipes
	addCraftingRecipe(GLASS,1,SAND,1);
	addCraftingRecipe(BEEF_COOKED,1,BEEF_RAW,1);
	addCraftingRecipe(CHICKEN_COOKED,1,CHICKEN_RAW,1);
	addCraftingRecipe(PORKCHOP_COOKED,1,PORKCHOP_RAW,1);
	addCraftingRecipe(DIAMOND,1,DIAMOND_ORE,1);
	addCraftingRecipe(INGOT_IRON,1,IRON_ORE,1);
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
