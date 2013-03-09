#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "zombieMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../message.h"
#include "../colision.h"
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
	notarget=0;
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
	notarget=0;
	timeTillWifiUpdate=rand()%4+4;
}
void zombieMob::updateMob(worldObject* world)
{
	if (animation==0) showGraphic(&zombieMobGraphic[0],x-world->CamX - (facing ? 10:0),y-world->CamY,facing ? true:false);
	else if (animation==1) 	showGraphic(&zombieMobGraphic[1],x-world->CamX - (facing ? 10:0),y-world->CamY,facing ? true:false);
	if (host==true)
	{
		target = mobHandlerFindMob(128,2,x,y);
		if (target->x < x && target->mobtype==2) facing = true;
		else if (target->mobtype==2) facing = false;
		jump++;
		if (colisions[0]==false) y+=vy;
		else vy=0;
		if (target->x > x-4 && target->x < x+4)
		{
			jump=0;
		}
		if (target->mobtype!=2)
		{
			notarget++;
			jump=0;
		}		
		else if (!colisions[1] && facing==false && !colisions[3] && jump>1)
		{
			x+= facing ? -1 : 1;
			jump=0;
		}
		else if (!colisions[2] && facing==true && !colisions[3] && jump>1)
		{
			x+= facing ? -1 : 1;
			jump=0;
		}
		else if ((colisions[1] || colisions[2]) && colisions[0])
		{
			vy=-2;
			y+=vy;
		}
		if (mobtype==2) notarget=0;
		ping=0;
		if (health<=0)
		{
			killMob();
		}
		if (notarget > 1800) killMob();
		if (animationclearframes==0) animation=0;
		else animationclearframes--;
		//iprintf("colisions = %d\n",colisions[0]);
		if (spritecol(x,y,target->x,target->y,sx,sy,target->sx,target->sy))
		{
			target->hurt(2,ZOMBIE_HURT);
		}
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
