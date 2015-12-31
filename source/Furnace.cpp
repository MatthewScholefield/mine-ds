#include "Furnace.h"

Furnace::Furnace(FILE* file) : Furnace(true)
{
	int readInUse;
	fscanf(file, "%d %d %d %d ", &readInUse, &fuelAmount, &timeTillFuelBurn, &fuelTillComplete);
	inUse = (bool)readInUse;
	source.loadFromFile(file);
	fuel.loadFromFile(file);
	result.loadFromFile(file);
}


void Furnace::saveToFile(FILE* file)
{
	fprintf(file, "%d %d %d %d ", inUse, fuelAmount, timeTillFuelBurn, fuelTillComplete);
	source.saveToFile(file);
	fuel.saveToFile(file);
	result.saveToFile(file);
}