#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "mobPlayer.h"
#include "../blockID.h"
#include <nds.h>
//ASDF?
Graphic playerMobGraphic[3];
playerMob::playerMob()
{
    x=0;
    y=0;
    vy=0;
    vx=0;
    alive=false;
}
playerMob::playerMob(int a,int b)
{
    x=a;
    y=b;
    vy=0;
    vx=0;
    alive=false;
	iprintf("%d,%d\n",a,b);
}
void playerMob::updateMob(worldObject* world)
{
	if (host)
	{
		world->CamX=x-256/2;
		world->CamY=y-192/2;
		if( world->CamX <0) world->CamX = 0;
		if (world->CamY<0) world->CamY = 0;
		if( world->CamX>WORLD_WIDTH*16-256) world->CamX = WORLD_WIDTH*16-256;
		if (world->CamY>(WORLD_HEIGHT+1)*16-192) world->CamY = (WORLD_HEIGHT+1)*16-192;
		if (keysHeld()&KEY_RIGHT) x++;
		if (keysHeld()&KEY_LEFT) x--;
		if (keysHeld()&KEY_UP) y--;
		if (keysHeld()&KEY_DOWN) y++;
	}
	if (x-world->CamX>-16 && x-world->CamX<256+16 && y-world->CamY>-32 && y-world->CamY<256)
	showGraphic(&playerMobGraphic[0],x-world->CamX,y-world->CamY);
}
void playerMob::sendWifiUpdate()
{
}
void playerMob::saveToFile(FILE* pFile)
{
}
void playerMob::loadFromFile(FILE* pFile)
{
}
bool canPlayerMobSpawnHere(worldObject* world,int x,int y)
{
	y++;
	if (!isBlockWalkThrough(world->blocks[x][y+1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y]!=CACTUS) return true;
	return false;
}
void playerMobInit()
{
	loadGraphic(&playerMobGraphic[0],true,0);
}

