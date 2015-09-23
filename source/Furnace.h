#pragma once
#include "InvBlock.h"

class Furnace
{
public:
	InvBlock sourceBlock, fuelBlock, resultBlock;
	int fuel, timeTillFuelBurn, fuelTillComplete;

	void saveToFile(FILE *file);

	Furnace(FILE *file);

	Furnace() : sourceBlock(), fuelBlock(), resultBlock(), fuel(0), timeTillFuelBurn(0), fuelTillComplete(0) { }

	Furnace(InvBlock source, InvBlock fuel)
	: sourceBlock(source), fuelBlock(fuel), resultBlock(), fuel(0), timeTillFuelBurn(0), fuelTillComplete(0) { }
};