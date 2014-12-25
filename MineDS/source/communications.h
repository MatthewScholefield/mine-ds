#pragma once
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
void recievedMobUpdate(int x,int y,int animation,int mobtype,int mobNum,bool f);
void placeBlock(int x,int y);
void matchBlocks(int x,int y,int block,int bgblock);
void recievePlaceBlock(int x,int y,int block,int block2);
void confirmBlock(int client_id,int x,int y);
void clientConfirmBlock(int x,int y);
void matchBlocksHost(int client_id,int x,int y,int block,int bgblock);
void killMob(int mobNum);
void wifiHurtMob(int mobNum,int amount,int type);
void recieveWorldUpdate();
bool canSpawnMob();
