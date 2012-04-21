#include "../world.h"
#include <nds.h>
#include "baseMob.h"
baseMob* mobs[10];
int stopcheck;
void mobHandlerInit()
{
	baseMobInit();
	stopcheck=0;
	int i;
	for (i=0;i<=10;i++)
	{
		mobs[i] = new baseMob();
		mobs[i] -> alive = false;
	}
}
void spawnBaseMob(worldObject* world)
{
	int i;
	for (i=0;i<=WORLD_HEIGHT;i++)
		if (canBaseMobSpawnHere(world,0,i))
		{
			iprintf("ASDF2");
			mobs[stopcheck]= new baseMob(0,i*16);
			stopcheck++;
		}
}
void spawnMob(int mobId,worldObject* world)
{
	iprintf("ASDF");
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
