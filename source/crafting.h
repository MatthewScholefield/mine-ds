#pragma once
#include <nds.h>
#include "blockID.h"
#include "InvBlock.h"

class Recipe
{
public:
	InvBlock createdblock;
	InvBlock neededblocks[4];

	Recipe() : createdblock(), neededblocks { }
	{
	}

	Recipe(int block, int amount, int aid, int aamount, int bid = AIR, int bamount = 0,
		int cid = AIR, int camount = 0, int did = AIR, int damount = 0) :
	createdblock(block, amount), neededblocks
	{
		{
			aid, aamount
		},
		{
			bid, bamount
		},
		{
			cid, camount
		},
		{
			did, damount
		}
	}
	{
	}
};
void craftingInit();
void addCraftingRecipe(int block, int amount, int aid, int aamount, int bid = AIR, int bamount = 0, int cid = AIR, int camount = 0, int did = AIR, int damount = 0);
void addFurnaceRecipe(int block, int amount, int aid, int aamount);
void craftingMenuInit();
int craftingMenuUpdate(touchPosition* touch);
