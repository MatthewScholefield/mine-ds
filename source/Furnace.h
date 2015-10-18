#pragma once
#include "InvBlock.h"

class Furnace
{
public:
	bool inUse;
	InvBlock sourceBlock, fuelBlock, resultBlock;
	int fuel, timeTillFuelBurn, fuelTillComplete;

	void saveToFile(FILE *file);

	Furnace(FILE *file);

	Furnace(bool inUse = false) : inUse(inUse), sourceBlock(), fuelBlock(), resultBlock(), fuel(0), timeTillFuelBurn(0), fuelTillComplete(0) { }

	Furnace(InvBlock source, InvBlock fuel)
	: inUse(true), sourceBlock(source), fuelBlock(fuel), resultBlock(), fuel(0), timeTillFuelBurn(0), fuelTillComplete(0) { }
};