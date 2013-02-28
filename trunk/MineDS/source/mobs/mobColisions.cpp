#include "../blockID.h"
#include "baseMob.h"
#include "hurt.h"
void colisionWithCactus(worldObject* world,baseMob* mob,int type,int x,int y,bool sub)
{
        if (mob->timeOnCactus>30) 
	{
		mob->hurt(1,CACTUS_HURT);
		mob->timeOnCactus=0;
	} 
	mob->timeOnCactus++;
}
void colision(worldObject* world,baseMob* mob,int type,int x,int y,bool sub)
{
	if(mob->host == true)
	{
		switch(world->blocks[x][y])
		{
			case CACTUS: colisionWithCactus(world,mob,type,x,y,sub); break;
		}
	}
}
