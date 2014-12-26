#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "MobMultiplayer.h"
#include "../blockID.h"
#include <nds.h>
//ASDF?
Graphic multiplayerMobGraphic[3];
MultiplayerMob::MultiplayerMob()
{
	x=0;
	y=0;
	vy=0;
	vx=0;
	alive=false;
	onground=false;
	animation=0;
	sx=6;
	sy=32;
	mobtype=2;
	ping=0;
}
MultiplayerMob::MultiplayerMob(int a,int b)
{
	gravity=3;
	gravityValue=3;
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
	ping=0;
}
void MultiplayerMob::hurt(int amount,int type)
{
	return; //Do nothing, if a Multiplayer mob is hurt then it should be realised on the server not client...
}
void MultiplayerMob::updateMob(WorldObject* world)
{
	if (x-world->CamX>-16 && x-world->CamX<256+16 && y-world->CamY>-32 && y-world->CamY<256)
	{
		if (animation==0) showGraphic(&multiplayerMobGraphic[0],x-world->CamX - (facing ? 10:0),y-world->CamY,facing ? true:false);
	}
	else if (animation==1) showGraphic(&multiplayerMobGraphic[1],x-world->CamX - (facing ? 10:0),y-world->CamY,facing ? true:false);
}
void MultiplayerMob::sendWifiUpdate()
{

}
void MultiplayerMob::saveToFile(FILE* pFile)
{
}
void MultiplayerMob::loadFromFile(FILE* pFile)
{
}
bool canMultiplayerMobSpawnHere(WorldObject* world,int x,int y)
{
	++y;
	if (!isBlockWalkThrough(world->blocks[x][y+1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y]!=CACTUS) return true;
	return false;
}
void multiplayerMobInit()
{
	loadGraphic(&multiplayerMobGraphic[0],true,0);
	loadGraphic(&multiplayerMobGraphic[1],true,1);
}

