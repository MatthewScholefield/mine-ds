#include <nds.h>
#include "inventory.h"
typedef struct
{
	InvBlock createdblock;
	InvBlock neededblocks[4];
	int amountOfNeededBlocks;
}recipe;
void craftingInit();
void addCraftingRecipe(int block,int amount,int aid,int aamount,int bid,int bamount,int cid,int camount,int did,int damount);
void addCraftingRecipe(int block,int amount,int aid,int aamount,int bid,int bamount,int cid,int camount);
void addCraftingRecipe(int block,int amount,int aid,int aamount,int bid,int bamount);
void addCraftingRecipe(int block,int amount,int aid,int aamount);
void addFurnaceRecipe(int block,int amount,int aid,int aamount);
void craftingMenuInit();
int craftingMenuUpdate(touchPosition* touch, unsigned char* oldX, unsigned char* oldY);
