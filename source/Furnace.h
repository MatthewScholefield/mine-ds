#pragma once
#include "InvBlock.h"

class Furnace
{
public:
	bool inUse;
	InvBlock source, fuel, result;
	int fuelAmount, timeTillFuelBurn, fuelTillComplete;

	void saveToFile(FILE *file);

	Furnace(FILE *file);

	Furnace(bool inUse = false) : inUse(inUse), source(), fuel(), result(), fuelAmount(0), timeTillFuelBurn(0), fuelTillComplete(0) { }

	Furnace(InvBlock source, InvBlock fuel)
	: inUse(true), source(source), fuel(fuel), result(), fuelAmount(0), timeTillFuelBurn(0), fuelTillComplete(0) { }
};