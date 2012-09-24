#include "world.h"
#include "mobs/baseMob.h"
int doHandshake();
void connectSuccess();
void communicationInit(worldObject* world2);
void recieveWorld(worldObject* world2);
void connectCode(int code2);
void setBlock(int x,int y,int block,int bgblock,int amount);
void sendblock(int a,int x, int y);
void sendMobUpdater(baseMob* mob,int mobNum);
void recievedMobUpdate(int x,int y,int animation,int mobtype,int mobNum);
