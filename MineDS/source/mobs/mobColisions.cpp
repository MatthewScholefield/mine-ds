#include "../blockID.h"
#include "baseMob.h"
#include "hurt.h"
int timeOnCactus;
void colisionWithCactus(worldObject* world,baseMob* mob,int type,int x,int y,bool sub)
{
        if (timeOnCactus>30) 
	{
		mob->hurt(1,CACTUS_HURT);
		timeOnCactus=0;
	} 
	timeOnCactus++;
}
void colision(worldObject* world,baseMob* mob,int type,int x,int y,bool sub)
{
	switch(world->blocks[x][y])
	{
		case CACTUS: colisionWithCactus(world,mob,type,x,y,sub); break;
	}
}
