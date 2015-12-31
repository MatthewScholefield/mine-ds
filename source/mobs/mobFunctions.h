#pragma once
#include "BaseMob.h"
void calculatePhysics(BaseMob *mob, bool inWater = false);
void calculateMiscData(World &world, BaseMob *mob);
void calculateMiscDataSmall(World &world, BaseMob *mob);
bool onScreen(int x, int y, int worldX, int worldY);