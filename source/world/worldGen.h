#pragma once
#include "WorldObject.h"
int extremeMountainGen(World &world, int startx, int starty, int endx);
void generateBedrock(World &world);
int flatGen(World &world, int startx, int starty, int endx);
void generateRandomBiome(World &world, int x, int endX);


void generateCaves(World &world);