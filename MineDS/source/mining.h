#pragma once
#include "world.h"
#include <nds.h>
#define NUM_BLOCKS 102
int getSelectedSlot();
void miningUpdate(worldObject* world, int sx, int sy, touchPosition touch, int keys);
void blocksCanPlace();
void setMiningDisabled(bool a);
void setSelectedSpace(int a);
