#pragma once
#include "world.h"
#include <nds.h>
#define NUM_BLOCKS 102
#define ACTIVATE_DELAY 20
void miningUpdate(WorldObject* world, touchPosition touch);
void enableMining();
void setMiningDisabled(bool a);
bool canMine();
