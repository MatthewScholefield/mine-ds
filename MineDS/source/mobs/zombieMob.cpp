#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "zombieMob.h"
#include "../blockID.h"
#include "../message.h"
//ASDF?
Graphic zombieMobGraphic[3];
zombieMob::zombieMob()
{
	x=0;
	y=0;
	vy=0;
	vx=0;
	ping=0;
	alive=false;
	onground=false;
	health=10;
	mobtype=0;
	animationclearframes=0;

}
zombieMob::zombieMob(int a,int b)
{	
	jump=0;
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
	facing=false;
	mobtype=3;
	health=20;
	ping=0;
	animation=0;
	timeTillWifiUpdate=rand()%4+4;
}
void zombieMob::updateMob(worldObject* world)
{
	if (animation==0) showGraphic(&zombieMobGraphic[0],x-world->CamX,y-world->CamY);
	else if (animation==1) showGraphic(&zombieMobGraphic[1],x-world->CamX,y-world->CamY);
	if (host==true)
	{
		jump++;
		if (colisions[0]==false) y+=vy;
		else vy=0;
		if (!colisions[1] && !colisions[3] && jump>1)
		{
			x++;
			jump=0;
		}
		else if (colisions[1] && colisions[0])
		{
			vy=-2;
			y+=vy;
		}
		ping=0;
		if (health<=0)
		{
			killMob();
		}
		if (animationclearframes==0) animation=0;
		else animationclearframes--;
		//iprintf("colisions = %d\n",colisions[0]);
	}
}
void zombieMob::sendWifiUpdate()
{
}
void zombieMob::saveToFile(FILE* pFile)
{
}
void zombieMob::loadFromFile(FILE* pFile)
{
}
bool canZombieMobSpawnHere(worldObject* world,int x,int y)
{
	y++;
	if (!isBlockWalkThrough(world->blocks[x][y+1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y]!=CACTUS && world->blocks[x][y+1]!=CACTUS) return true;
	return false;
}
void zombieMobInit()
{
	loadGraphic(&zombieMobGraphic[0],true,3);
	loadGraphic(&zombieMobGraphic[1],true,4);
}
void zombieMob::hurt(int amount,int type)
{
	
	if (animation==1)
		return;
	if (type!=VOID_HURT)
		vy-=2;
	y+=vy;
	health-=amount;
	animation=1;
	animationclearframes=20;
}
bool zombieMob::isMyPlayer()
{
	return false;
}
