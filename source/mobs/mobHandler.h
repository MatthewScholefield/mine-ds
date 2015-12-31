#pragma once
#include "../blockUpdaters/WaterUpdater.h"
#include "../world/World.h"
#include "BaseMob.h"
#include <fat.h>

#define JUMP_VELOCITY -6.0
#define WALK_VELOCITY 2.0
#define RUN_VELOCITY 3.0

void createWaterMob(int x, int y, int level = MAX_WATER_LEVEL);
void createItemMob(int x, int y, int blockID, int amount = 1, int displayID = -1, float initVX = 54321.f);
bool canMobSpawnHere(World &world, int x, int y);
int getDefaultSpawnX();
void saveMobs(FILE* f);
void loadMobs(FILE* f);
int spawnMobAt(MobType type, int x, int y);
void mobHandlerInit();
void mobHandlerUpdate(World &world, touchPosition &touch);
void mobsReset();
BaseMob::Ptr getPlayerPtr();
BaseMob::Ptr isMobAt(int x, int y);
BaseMob::Ptr mobHandlerFindMob(int range, MobType type, int x, int y);
int getPlayerX();
void triggerPlayerRespawn();