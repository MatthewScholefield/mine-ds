#include "../world.h"
void mobHandlerInit();
void mobHandlerUpdate(worldObject* world);
void mobHandlerKillMob(int a);
void mobHandlerReadWifiUpdate(int x,int y,int animation,int mobtype,int mobNum,worldObject* world,bool f);
void mobsReset();
void mobHandlerRespawnPlayer();
void mobHandlerHurtMob(int mobNum,int amount, int type);
int isMobAt(int x,int y);
void mobHandlerHurtMobWifi(int mobNum,int amount, int type);
