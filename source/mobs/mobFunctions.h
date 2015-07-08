#pragma once
#include "BaseMob.h"
void calculateMiscData(WorldObject* world, BaseMob_ptr mob);
void calculateMiscDataSmall(WorldObject* world, BaseMob_ptr mob);
bool onScreen(int x, int y, int worldX, int worldY);