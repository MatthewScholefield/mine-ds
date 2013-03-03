#include "../world.h"
#include <nds.h>
#include <stdio.h>
#include "baseMob.h"
#include "mobPlayer.h"
#include "mobFunctions.h"
#include "../nifi.h"
#include "../communications.h"
#include "mobMPlayer.h"
#include "../deathScreen.h"
baseMob* mobs[100];
bool hasSpawnPlayer;
int playerId;
bool spawnPlayerAtPos;
bool playerDeathRespawn;
int spawn_x,spawn_y;
void mobHandlerRespawnPlayer()
{
	delete mobs[playerId];
	mobs[playerId] = new baseMob();
	playerDeathRespawn = false;
}
void mobHandlerKillMob(int mobNum)
{
	mobs[mobNum]->killMob();
}
void mobsReset()
{
	int i;
	for (i=0;i<=100;i++)
	{
		//delete mobs[i];
		mobs[i] = new baseMob();
		mobs[i] -> killMob();
	}
	hasSpawnPlayer=false;
	spawnPlayerAtPos=false;
}
void mobHandlerInit()
{
	baseMobInit();
	playerMobInit();
	MplayerMobInit();
	int i;
	for (i=0;i<=100;i++)
	{
		mobs[i] = new baseMob();
		mobs[i] -> killMob();
	}
	hasSpawnPlayer=false;
	spawnPlayerAtPos=false;
}
int findFreeMobSpawnNum()
{
	int i;
	for (i=1;i<=100;i++)
		if (mobs[i]->alive==false && mobs[i]->isMyPlayer() == false)
			return i;
	return -1; 
}
bool canMobSpawnHere(int mobId,worldObject* world, int a, int b)
{	
	switch(mobId)
	{
		case 0: return canBaseMobSpawnHere(world,a,b); break;
		case 1: return canPlayerMobSpawnHere(world,a,b); break;
		case 2: return canPlayerMobSpawnHere(world,a,b); break;
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
				delete mobs[mobNum]; //Free Memory and Stop Crashes
				switch(mobId)
				{
					case 0: mobs[mobNum]= new baseMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
					case 1: mobs[mobNum]= new playerMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
					case 2: mobs[mobNum]= new MplayerMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
					default: break;
				}
				mobs[mobNum]->host=true;
			}
			else printf("Can't find number\n");
			i=WORLD_HEIGHT+1;
			j=WORLD_WIDTH+1;
		}
}
void spawnMobAt(int mobId,worldObject* world,int x,int y)
{	int mobNum=findFreeMobSpawnNum();
	if (mobNum!=-1)
	{
		delete mobs[mobNum]; //Free Memory and Stop Crashes
		switch(mobId)
		{
			case 0: mobs[mobNum]= new baseMob(x,y); mobs[mobNum]->unKillMob(); break;
			case 1: mobs[mobNum]= new playerMob(x,y); mobs[mobNum]->unKillMob(); break;
			case 2: mobs[mobNum]= new MplayerMob(x,y); mobs[mobNum]->unKillMob(); break;
			default: break;
		}
		mobs[mobNum]->host=true;
	}
}
void spawnMob(int mobId,worldObject* world,int mobNum)
{
	int i;
	int j;
	for (j=0;j<=WORLD_WIDTH;j++)
	for (i=0;i<=WORLD_HEIGHT;i++)
		if (canMobSpawnHere(mobId,world,j,i))
		{
			delete mobs[mobNum]; //Free Memory and Stop Crashes
			switch(mobId)
			{
				case 0: mobs[mobNum]= new baseMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
				case 1: mobs[mobNum]= new playerMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
				case 2: mobs[mobNum]= new MplayerMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
				default: break;
			}
			mobs[mobNum]->host=false;
			i=WORLD_HEIGHT+1;
			j=WORLD_WIDTH+1;
		}
}
void mobHandlerReadWifiUpdate(int x,int y,int animation,int mobtype,int mobNum,worldObject* world,bool facing)
{
	//printf("Recieved mob update! - %d, %d\n", mobNum,mobtype);
	if (mobs[mobNum]->mobtype!=mobtype)
	{	
		if (mobs[mobNum]->mobtype==1)
		{
			spawnMobAt(1,world,mobs[mobNum]->x,mobs[mobNum]->y);
		}
		spawnMob(mobtype,world,mobNum);
	}
	mobs[mobNum]->unKillMob();
	mobs[mobNum]->x = x;
	mobs[mobNum]->y = y;
	mobs[mobNum]->animation = animation;
	mobs[mobNum]->host = false;
	mobs[mobNum]->facing = facing;
	mobs[mobNum]->ping=0;
	//:D
}
void mobHandlerUpdate(worldObject* world)
{
	if (!hasSpawnPlayer || !playerDeathRespawn)
	{
		spawnMob(1,world);
		hasSpawnPlayer=true;
		playerDeathRespawn = true;
	}
	int i;
	for(i=1;i<100;i++)
	{
		if (mobs[i]->isMyPlayer())
		{
			playerId=i;
			if (mobs[i]->alive==false && playerDeathRespawn == true)
			{
				deathScreenSetup();
			}		
		}
		if (mobs[i]->alive==true)
		{
			calculateMiscData(world,mobs[i]);
			mobs[i]->updateMob(world);
			mobs[i]->timeTillWifiUpdate--;
			if (isWifi())
			{
				mobs[i]->ping++;
				if(mobs[i]->ping>80)
				{
					mobs[i]->ping = 0;
					mobs[i]->alive=false;
				}
			}
		}
		if (mobs[i]->timeTillWifiUpdate==0 && isWifi())
		{
			
			if (mobs[i]->host==true)
			{
				sendMobUpdater(mobs[i],i);
				mobs[i]->timeTillWifiUpdate = rand() % 3 + 3;
				
			}
			
		}
		else if (mobs[i]->timeTillWifiUpdate==0) mobs[i]->timeTillWifiUpdate = 255;
	}
	if (keysDown() & KEY_Y) spawnMob(0,world);

}
