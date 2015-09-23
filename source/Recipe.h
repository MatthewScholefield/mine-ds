#pragma once
#include "blockID.h"
#include "InvBlock.h"

class Recipe
{
public:
	InvBlock result;
	InvBlock needed[4];
	Recipe() : result(), needed { }
	{
	}

	Recipe(int resultID, int resultAmount, int aID, int aAmount, int bID = AIR, int bAmount = 0,
		int cID = AIR, int cAmount = 0, int dID = AIR, int dAmount = 0) :
	result(resultID, resultAmount), needed
	{
		{
			aID, aAmount
		},
		{
			bID, bAmount
		},
		{
			cID, cAmount
		},
		{
			dID, dAmount
		}
	}
	{
	}
};

class FurnaceRecipe
{
public:
	int fuel;
	int result, needed;

	FurnaceRecipe(int fuel, int result, int needed)
	: fuel(fuel), result(result), needed(needed) { }
};
