#pragma once
#include "world.h"

void createFurnace(WorldObject *world, int x, int y, bool bg);
void destroyFurnace(WorldObject *world, int x, int y, bool bg);
void openFurnace(WorldObject *world, int x, int y, bool bg);
void closeFurnace();
int getOpenedFurnaceID();