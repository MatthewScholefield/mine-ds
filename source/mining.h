#pragma once
#include "world/WorldObject.h"
#include <nds.h>
#define NUM_BLOCKS 110
#define ACTIVATE_DELAY 20
#define HIT_SOUND_DELAY 10
void miningUpdate(World &world, touchPosition touch);
void enableMining();
void setMiningDisabled(bool a);
bool canMine();
void destroyBlock(World &world, int x, int y, bool bg, bool byHand = true);