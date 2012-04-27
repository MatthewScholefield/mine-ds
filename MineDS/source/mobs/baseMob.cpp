#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "baseMob.h"
#include "../blockID.h"
//ASDF?
Graphic baseMobGraphic[3];
baseMob::baseMob()
{
    x=0;
    y=0;
    vy=0;
    vx=0;
    alive=true;
}
baseMob::baseMob(int a,int b)
{
    x=a;
    y=b;
    vy=0;
    vx=0;
    alive=true;
}
void baseMob::resetVelocity()
{
    vy=0;
    vx=0;
}
void baseMob::updateMob(worldObject* world)
{
	iprintf("BaseMob Updating!\n");
	 showGraphic(&baseMobGraphic[0],x-world->CamX,y-world->CamY);
}
void baseMob::setXYPos(int a,int b)
{
    x=a;
    y=b;
}
void baseMob::sendWifiUpdate()
{
}
void baseMob::saveToFile(FILE* pFile)
{
}
void baseMob::loadFromFile(FILE* pFile)
{
}
bool canBaseMobSpawnHere(worldObject* world,int x,int y)
{
	y++;
	if (!isBlockWalkThrough(world->blocks[x][y+1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y]!=CACTUS) return true;
	return false;
}
void baseMobInit()
{
	loadGraphic(&baseMobGraphic[0],true,0);
}
