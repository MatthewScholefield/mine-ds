#include "../world.h"
#include <nds.h>
#include "baseMob.h"
baseMob* mobs[100];
int stopcheck;
void mobHandlerInit()
{
	baseMobInit();
	stopcheck=0;
	int i;
	for (i=0;i<=100;i++)
	{
		mobs[i] = new baseMob();
		mobs[i] -> alive = false;
	}
}
int findFreeMobSpawn()
{
	int i;
	for (i=0;i<=100;i++)
		if (mobs[i]->alive==false)
			return i;
	return -1; 
}
void spawnBaseMob(worldObject* world)
{
	int i;
	int j;
	for (j=0;j<=WORLD_WIDTH;j++)
	for (i=0;i<=WORLD_HEIGHT;i++)
		if (canBaseMobSpawnHere(world,j,i))
		{
			int mobId=findFreeMobSpawn();
			if (mobId!=-1)
			{
				mobs[mobId]= new baseMob(j*16,i*16);
			}
			i=WORLD_HEIGHT+1;
			j=WORLD_WIDTH+1;
		}
}
void spawnMob(int mobId,worldObject* world)
{
	switch(mobId)
	{
		case 0: spawnBaseMob(world); break;
	}
}
void mobHandlerUpdate(worldObject* world)
{
	int i;
	for(i=0;i<=stopcheck;i++)
	{
		if (mobs[i]->alive==true)
		{
		 mobs[i]->updateMob(world);
		}
	}
	if (keysDown() & KEY_Y) spawnMob(0,world);
}
