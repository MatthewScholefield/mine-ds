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
	onground=false;
	animation=0;
	mobtype=2;
	health=20;
}

playerMob::playerMob(int a,int b)
{
	gravity=4;
	gravityValue=4;
	sx=6;
	sy=32;
	x=a;
	y=b;
	vy=0;
	vx=0;
	alive=false;
	onground=false;
	animation=0;
	mobtype=2;
	health=20;
	timeTillWifiUpdate=rand()%4+4;
}
void playerMob::hurt(int amount,int type)
{
	health-=amount;
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
		if (keysHeld()&KEY_RIGHT && !colisions[1] && !colisions[3]){ x++; animation=0;}
		if (keysHeld()&KEY_LEFT && !colisions[2] && !colisions[3])
{ x--; animation=1; }
		if (colisions[3]==true)
		{
			vy=0;
			y+=1;
		}
		if (colisions[0]==false)
		{
		 y+=vy; 
		}
		else vy=0;
		if ((keysDown() & KEY_UP || keysDown() & KEY_A) && colisions[0]==true && !colisions[3]) vy=-2;	y+=vy;	
	}
	if (x-world->CamX>-16 && x-world->CamX<256+16 && y-world->CamY>-32 && y-world->CamY<256)
	showGraphic(&playerMobGraphic[0],x-world->CamX - (animation ? 10:0),y-world->CamY,animation ? true:false);
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

