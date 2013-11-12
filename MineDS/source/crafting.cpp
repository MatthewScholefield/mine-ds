#include "mining.h" //for NUM_BLOCKS
#include "graphics/graphics.h"
#include "graphics/inventoryGraphics.h"
#include "graphics/subBgHandler.h"
#include "titlescreen.h"
#include "inventory.h"
#include "blockID.h"
#include "message.h"
#include "titlescreen.h" // for isSurvival
#include <nds.h> // for keysDown()
#include <stdio.h>
#include "crafting.h"
recipe craftingRecipes[128];
int nextCraftingRecipe = 0;
Graphic resultBlock;
Graphic neededblocks[4];
int currentViewingRecipe = 0;
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
	//addCraftingRecipe(INGOT_IRON,1,IRON_ORE,1,DIAMOND_ORE,1,PORKCHOP_RAW,1,CHICKEN_RAW,1); // For testing
	loadGraphic(&resultBlock,0,0);
	for (int i = 0; i<=3;i++)
	{
		loadGraphicSub(&neededblocks[i],2,0);
	}
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
void updateCraftingGraphics()
{
	unloadGraphic(&resultBlock);
	loadGraphicSub(&resultBlock,2,craftingRecipes[currentViewingRecipe].createdblock.blockId);
	for (int i = 0; i<=3;i++)
	{

		unloadGraphic(&neededblocks[i]);
		loadGraphicSub(&neededblocks[i],2,craftingRecipes[currentViewingRecipe].neededblocks[i].blockId);
	}
}
void craftingMenuInit()
{
	disableInvGraphics();
	lcdMainOnTop();
	consoleClear();
	drawBackground();
	miningSetScene(true);
	drawButton(24,17,6);
	printf("\x1b[18;25HCraft");
	drawButton(28,10,2);
	printf("\x1b[11;29H%c",16);
	drawButton(0,10,2);
	printf("\x1b[11;1H%c",17);
	currentViewingRecipe = 0;
	updateCraftingGraphics();
}
int craftingMenuUpdate()
{
	touchPosition touch;
	touchRead(&touch);
	showGraphic(&resultBlock,180,84);
	showGraphic(&neededblocks[0],60,84);
	showGraphic(&neededblocks[1],60,84-16);
	showGraphic(&neededblocks[2],60,84+16);
	showGraphic(&neededblocks[3],60,84+32);
	//Created block
	printf("\x1b[11;25Hx%d",craftingRecipes[currentViewingRecipe].createdblock.blockAmount);
	//First needed block (top middle)
	printf("\x1b[11;10H%d/%d  ",checkInventory(craftingRecipes[currentViewingRecipe].neededblocks[0].blockId),craftingRecipes[currentViewingRecipe].neededblocks[0].blockAmount);	
	//Second needed block (top)
	if (craftingRecipes[currentViewingRecipe].neededblocks[1].blockAmount > 0)
		printf("\x1b[9;10H%d/%d  ",checkInventory(craftingRecipes[currentViewingRecipe].neededblocks[1].blockId),craftingRecipes[currentViewingRecipe].neededblocks[1].blockAmount);
	else
		printf("\x1b[9;10H       ");
	//Third needed block (top)
	if (craftingRecipes[currentViewingRecipe].neededblocks[2].blockAmount > 0)
		printf("\x1b[13;10H%d/%d  ",checkInventory(craftingRecipes[currentViewingRecipe].neededblocks[2].blockId),craftingRecipes[currentViewingRecipe].neededblocks[2].blockAmount);
	else
		printf("\x1b[13;10H       ");
	//Forth needed block (top)
	if (craftingRecipes[currentViewingRecipe].neededblocks[3].blockAmount > 0)
		printf("\x1b[15;10H%d/%d  ",checkInventory(craftingRecipes[currentViewingRecipe].neededblocks[3].blockId),craftingRecipes[currentViewingRecipe].neededblocks[3].blockAmount);
	else
		printf("\x1b[15;10H       ");
	
	
	if (keysDown() & KEY_Y)
	{
		return 1;
	}
	if (touch.px > 29*8 && touch.py > 11*8 && touch.py<14*8 && keysDown() & KEY_TOUCH){
		currentViewingRecipe++;
		if (currentViewingRecipe >= nextCraftingRecipe)
			currentViewingRecipe = 0;
		updateCraftingGraphics();
 	}
	if (touch.px < 24 && touch.py > 11*8 && touch.py<14*8 && keysDown() & KEY_TOUCH)
	{
		currentViewingRecipe--;
		if (currentViewingRecipe < 0)
			currentViewingRecipe = nextCraftingRecipe - 1;
		updateCraftingGraphics();
	}
        return 0;
}
