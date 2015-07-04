#pragma once
#include "../world.h"
#include "BaseMob.h"
#include <fat.h>

#define JUMP_VELOCITY -6.0
#define WALK_VELOCITY 2.0
#define RUN_VELOCITY 3.0

bool canMobSpawnHere(WorldObject *world, int x, int y);
int getDefaultSpawnX();
void saveMobs(FILE* f);
void loadMobs(FILE* f);
int spawnMobAt(int mobId, WorldObject* world, int x, int y);
void mobHandlerInit();
void mobHandlerUpdate(WorldObject* world);
void mobHandlerKillMob(int a);
void mobHandlerReadWifiUpdate(int x, int y, int animation, int mobtype, int mobNum, WorldObject* world, bool f);
void mobsReset(bool playerSpawned = false);
void mobHandlerHurtMob(int mobNum, int amount, int type);
int isMobAt(int x, int y);
void mobHandlerHurtMobWifi(int mobNum, int amount, int type);
BaseMob* mobHandlerFindMob(int range, int type, int x, int y);
int getPlayerX();