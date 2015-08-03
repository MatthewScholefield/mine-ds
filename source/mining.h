#pragma once
#include "world.h"
#include <nds.h>
#define NUM_BLOCKS 104
#define ACTIVATE_DELAY 20
#define HIT_SOUND_DELAY 10
void miningUpdate(WorldObject* world, touchPosition touch);
void enableMining();
void setMiningDisabled(bool a);
bool canMine();
