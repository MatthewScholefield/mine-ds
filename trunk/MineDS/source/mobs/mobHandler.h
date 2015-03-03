#include "../world.h"
#include "baseMob.h"
#include <fat.h>

#define JUMP_VELOCITY 2.5

int getDefaultSpawnX();
void saveMobs(FILE* f);
void loadMobs(FILE* f);
int spawnMobAt(int mobId, worldObject* world, int x, int y);
void mobHandlerInit();
void mobHandlerUpdate(worldObject* world);
void mobHandlerKillMob(int a);
void mobHandlerReadWifiUpdate(int x,int y,int animation,int mobtype,int mobNum,worldObject* world,bool f);
void mobsReset(bool playerSpawned = false);
void mobHandlerHurtMob(int mobNum,int amount, int type);
int isMobAt(int x,int y);
void mobHandlerHurtMobWifi(int mobNum,int amount, int type);
baseMob* mobHandlerFindMob(int range,int type,int x,int y);