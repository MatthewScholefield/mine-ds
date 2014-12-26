#pragma once
#include "../world.h"
#include "BaseMob.h"
#include <fat.h>
int getDefaultSpawnX();
void savePlayer(FILE *data);
void loadPlayer(FILE *data);
void mobHandlerInit();
void mobHandlerUpdate(WorldObject* world);
void mobHandlerKillMob(int a);
void mobHandlerReadWifiUpdate(int x,int y,int animation,int mobtype,int mobNum,WorldObject* world,bool f);
void mobsReset(bool playerSpawned = false);
void mobHandlerHurtMob(int mobNum,int amount, int type);
int isMobAt(int x,int y);
void mobHandlerHurtMobWifi(int mobNum,int amount, int type);
BaseMob* mobHandlerFindMob(int range,int type,int x,int y);
