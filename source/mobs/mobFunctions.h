#pragma once
#include "BaseMob.h"
void calculatePhysics(BaseMob *mob, bool inWater = false);
void calculateMiscData(WorldObject* world, BaseMob *mob);
void calculateMiscDataSmall(WorldObject* world, BaseMob *mob);
bool onScreen(int x, int y, int worldX, int worldY);