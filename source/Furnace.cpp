#include "Furnace.h"

Furnace::Furnace(FILE* file)
{
	fscanf(file, "%d %d %d ", &fuel, &timeTillFuelBurn, &fuelTillComplete);
	sourceBlock.loadFromFile(file);
	fuelBlock.loadFromFile(file);
	resultBlock.loadFromFile(file);
}


void Furnace::saveToFile(FILE* file)
{
	fprintf(file, "%d %d %d ", fuel, timeTillFuelBurn, fuelTillComplete);
	sourceBlock.saveToFile(file);
	fuelBlock.saveToFile(file);
	resultBlock.saveToFile(file);
}