#include "Furnace.h"

Furnace::Furnace(FILE* file) : Furnace(true)
{
	fscanf(file, "%d %d %d ", &fuleAmount, &timeTillFuelBurn, &fuelTillComplete);
	source.loadFromFile(file);
	fuel.loadFromFile(file);
	result.loadFromFile(file);
}


void Furnace::saveToFile(FILE* file)
{
	fprintf(file, "%d %d %d ", fuleAmount, timeTillFuelBurn, fuelTillComplete);
	source.saveToFile(file);
	fuel.saveToFile(file);
	result.saveToFile(file);
}