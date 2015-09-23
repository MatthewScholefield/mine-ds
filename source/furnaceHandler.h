#pragma once
#include "world.h"

void convertItemToFuel(Furnace &furnace);
void createFurnace(WorldObject &world, int x, int y, bool bg);
void destroyFurnace(WorldObject &world, int x, int y, bool bg);
void openFurnace(WorldObject &world, int x, int y, bool bg);
void closeFurnace();
int getOpenedFurnaceID();
void createResult(Furnace &furnace);
int fuelNeeded(const Furnace &furnace);
void saveFurnaces(FILE *file, WorldObject &world);
void loadFurnaces(FILE *file, WorldObject &world);

inline int getFurnaceID(WorldObject &world, int x, int y, bool bg)
{
	return (world.data[x][y] & (bg ? 0x000F0000 : 0x0000000F)) >> (bg ? 4 * 4 : 0);
}