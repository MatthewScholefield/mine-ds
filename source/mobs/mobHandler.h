#pragma once
#include "../world.h"
#include "BaseMob.h"
#include <fat.h>

#define JUMP_VELOCITY -6.0
#define WALK_VELOCITY 2.0
#define RUN_VELOCITY 3.0

void createItemMob(int x, int y, int blockID, int amount = 1, int displayID = -1, float initVX = 54321.f);
bool canMobSpawnHere(WorldObject *world, int x, int y);
int getDefaultSpawnX();
void saveMobs(FILE* f);
void loadMobs(FILE* f);
int spawnMobAt(MobType type, int x, int y);
void mobHandlerInit();
void mobHandlerUpdate(WorldObject* world, touchPosition *touch = NULL);
void mobsReset(bool playerSpawned = false);
BaseMob_ptr isMobAt(int x, int y);
BaseMob_ptr mobHandlerFindMob(int range, MobType type, int x, int y);
int getPlayerX();