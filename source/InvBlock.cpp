#include "InvBlock.h"

void InvBlock::saveToFile(FILE* file)
{
	fprintf(file, "%d %d ", ID, amount);
}

void InvBlock::loadFromFile(FILE* file)
{
	fscanf(file, "%d %d ", &ID, &amount);
}