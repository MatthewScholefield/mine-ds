#pragma once
#include "InvBlock.h"

class Furnace
{
public:
	bool inUse;
	InvBlock source, fuel, result;
	int fuleAmount, timeTillFuelBurn, fuelTillComplete;

	void saveToFile(FILE *file);

	Furnace(FILE *file);

	Furnace(bool inUse = false) : inUse(inUse), source(), fuel(), result(), fuleAmount(0), timeTillFuelBurn(0), fuelTillComplete(0) { }

	Furnace(InvBlock source, InvBlock fuel)
	: inUse(true), source(source), fuel(fuel), result(), fuleAmount(0), timeTillFuelBurn(0), fuelTillComplete(0) { }
};