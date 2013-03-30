#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "cowMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../message.h"
#include "../collision.h"
Graphic cowMobGraphic[2];

cowMob::cowMob()
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
	smallmob=true;
}
cowMob::cowMob(int a,int b)
{	
	jump=0;
	gravity=3;
	gravityValue=3;
	sx=10;
	sy=16;
	x=a;
	y=b;
	vy=0;
	vx=0;
	alive=false;
	onground=false;
	facing=false;
	mobtype=4;
	health=10;
	ping=0;
	animation=0;
	notarget=0;
	timeTillWifiUpdate=rand()%4+4;
	smallmob=true;
}
void cowMob::updateMob(worldObject* world)
{

	if (rand()%2 == 1 && mov == 1)
		dir = true;
	else if (mov == 1)
		dir = false;

	if (animation==0) showGraphic(&cowMobGraphic[0],x-world->CamX,y-world->CamY,facing ? true:false);
	else if (animation==1) 	showGraphic(&cowMobGraphic[1],x-world->CamX,y-world->CamY,facing  ? true:false);

	if (host==true)
	{
		target = mobHandlerFindMob(128,2,x,y);

		if (target->x < x && target->mobtype==2 && scared) facing = false; //Face away from the player when scared
		else if (target->mobtype==2 && scared) facing = true;
		jump++;
		if (collisions[0]==false) y+=vy;
		else vy=0;
		if (collisions[3])
		{
			vy=0;
			y+=3;
		}
		if (scaredtimer > 125)
		{
			scared = false;
			scaredtimer = 0;
		}

		if (target->mobtype!=2)
		{
			scared = false;
			notarget++;
			jump=0;
		}

		else if (!scared)
		{
			if (rand()%50 == 17 || mov < 60) //2% chance that the cow will move from 60 pixels. The 26 can be any number between 0 and 99.
			{
				if (dir)
					facing = true;
				else
					facing = false;
				if (!collisions[1] && !dir && jump>1)
				{
					x += dir ? -1 : 1;
					jump=0;
				}
				if (!collisions[2] && dir && jump>1)
				{
					x += dir ? -1 : 1;
					jump=0;
				}
				else if ((collisions[1] || collisions[2]) && collisions[0]  && !collisions[3])
				{
					vy=-2;
					y+=vy;
				}
				if (mov > 59)
					mov = 0;
				mov++;
			}
		}

		else if (!collisions[1] && !facing && jump)
		{
			x+= facing ? -1 : 1;
			scaredtimer++;
			jump=0;
		}
		else if (!collisions[2] && facing && jump)
		{
			x+= facing ? -1 : 1;
			scaredtimer++;
			jump=0;
		}
		else if ((collisions[1] || collisions[2]) && collisions[0] && !collisions[3])
		{
			scaredtimer++;
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
		iprintf("\x1b[0;0Hcollisions = %d,%d,%d,%d\n",collisions[0],collisions[1],collisions[2],collisions[3]);
	}
}
void cowMob::sendWifiUpdate()
{
}
void cowMob::saveToFile(FILE* pFile)
{
}
void cowMob::loadFromFile(FILE* pFile)
{
}
bool canCowMobSpawnHere(worldObject* world,int x,int y)
{
	y++;
	if (!isBlockWalkThrough(world->blocks[x][y+1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y]!=CACTUS && world->blocks[x][y+1]!=CACTUS) return true;
	return false;
}
void cowMobInit()
{
	loadGraphic(&cowMobGraphic[0],true,12,16,16);
	loadGraphic(&cowMobGraphic[1],true,13,16,16);
}
void cowMob::hurt(int amount,int type)
{
	
	if (animation==1)
		return;
	if (type!=VOID_HURT)
		vy-=2;
	y+=vy;
		health-=amount;
	if (type == PLAYER_HURT)
		scared = true;
	animation=1;
	animationclearframes=20;
}
bool cowMob::isMyPlayer()
{
	return false;
}
