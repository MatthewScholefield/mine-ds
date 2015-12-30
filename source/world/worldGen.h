#pragma once
#include "WorldObject.h"
int extremeMountainGen(WorldObject &world, int startx, int starty, int endx);
void generateBedrock(WorldObject &world);
int flatGen(WorldObject &world, int startx, int starty, int endx);
void generateRandomBiome(WorldObject &world, int x, int endX);


void generateCaves(WorldObject &world);