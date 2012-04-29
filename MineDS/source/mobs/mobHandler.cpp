#include "../world.h"
#include <nds.h>
#include "baseMob.h"
baseMob* mobs[100];
void mobHandlerInit()
{
	baseMobInit();
	int i;
	for (i=0;i<=100;i++)
	{
		mobs[i] = new baseMob();
		mobs[i] -> killMob();
	}
}
int findFreeMobSpawnNum()
{
	int i;
	for (i=0;i<=100;i++)
		if (mobs[i]->alive==false)
			return i;
	return -1; 
}
bool canMobSpawnHere(int mobId,worldObject* world, int a, int b)
{	
	switch(mobId)
	{
		case 0: return canBaseMobSpawnHere(world,a,b); break;
		default: break;
	}
	return false;
}
void spawnMob(int mobId,worldObject* world)
{
	int i;
	int j;
	for (j=0;j<=WORLD_WIDTH;j++)
	for (i=0;i<=WORLD_HEIGHT;i++)
		if (canMobSpawnHere(mobId,world,j,i))
		{
			int mobNum=findFreeMobSpawnNum();
			if (mobNum!=-1)
			{
				
				switch(mobId)
				{
					case 0: mobs[mobNum]= new baseMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
					default: break;
				}
				mobs[mobNum]->host=true;
			}
			i=WORLD_HEIGHT+1;
			j=WORLD_WIDTH+1;
		}
}
void mobHandlerUpdate(worldObject* world)
{
	int i;
	for(i=1;i<100;i++)
	{
		if (mobs[i]->alive==true)
		{
			mobs[i]->updateMob(world);
		}
	}
	if (keysDown() & KEY_Y) spawnMob(0,world);
}
