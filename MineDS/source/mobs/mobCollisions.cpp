#include "../blockID.h"
#include "baseMob.h"
#include "hurt.h"
#include <nds.h>
void collisionWithCactus(worldObject* world,baseMob* mob,int type,int x,int y,bool sub)
{
        if (mob->timeOnCactus>30) 
	{
		mob->hurt(1,CACTUS_HURT);
		mob->timeOnCactus=0;
	} 
	mob->timeOnCactus++;
}
void collision(worldObject* world,baseMob* mob,int type,int x,int y,bool sub)
{
	if(mob->host == true)
	{
		switch(world->blocks[x][y])
		{
			case CACTUS: collisionWithCactus(world,mob,type,x,y,sub); break;
		}
		if (!(keysHeld() & KEY_DOWN))
		{
			switch(world->bgblocks[x][y])
			{
				case CACTUS: collisionWithCactus(world,mob,type,x,y,sub); break;
			}
		}
	}
}
