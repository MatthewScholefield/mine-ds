#include "../world.h"
#include <nds.h>
#include <stdio.h>
#include "baseMob.h" //Bad guys are nameMob
#include "mobPlayer.h" //Good guys are mobName
#include "mobMPlayer.h"
#include "zombieMob.h"
#include "pigMob.h"
#include "cowMob.h"
#include "sheepMob.h"
#include "mobFunctions.h"
#include "../nifi.h"
#include "../communications.h"
#include "../collision.h"
#include "../deathScreen.h"
#include "../message.h"
baseMob* mobs[100];
const int peacefulmobs[] = {4,5,6}; 
bool hasSpawnPlayer;
#define playerId 1
bool spawnPlayerAtPos;
int spawn_x,spawn_y;

baseMob* mobHandlerFindMob(int range,int type,int x,int y)
{
	int closest=range * range + 1;
	int mobNum=-1;
	int i;
	for (i=0;i<=100;i++)
	{
		if (mobs[i]->mobtype==type)
		{
			if (mobs[i]->x > x-range && mobs[i]->x < x+range)
				if(mobs[i]->y > y-range && mobs[i]->y < y+range)
				{
					int tmp = abs(mobs[i]->x - x) + abs(mobs[i]->y - y);
					if (tmp < closest)
					{
						closest = tmp;
						mobNum = i;
					}
				} 
		}
	}
	if (mobNum != -1)
	{
		mobs[mobNum]->mobId=mobNum;
		return mobs[mobNum];
	}
}
void mobHandlerHurtMobWifi(int mobNum,int amount, int type)
{
	if (mobs[mobNum]->host==true)
	{
		mobs[mobNum]->hurt(amount,type);
	}
}
void mobHandlerHurtMob(int mobNum,int amount, int type)
{
	if (mobs[mobNum]->host==true)
	{
		mobs[mobNum]->hurt(amount,type);
	}	
	else if (isWifi())
	{
		wifiHurtMob(mobNum,amount,type);
	}
}
int isMobAt(int x,int y)
{
	int i;
	for (i=0;i<=100;i++)
	{
		if (spritecol(mobs[i]->x,mobs[i]->y,x,y,mobs[i]->sx,mobs[i]->sy,4,4) && mobs[i]->alive==true)
		{
			return i;
		}
	}
	return -1;
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
	zombieMobInit();
	pigMobInit();
	cowMobInit();
	sheepMobInit();
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
		case 3: return canZombieMobSpawnHere(world,a,b); break;
		case 4: return canPigMobSpawnHere(world,a,b); break;
		case 5: return canCowMobSpawnHere(world,a,b); break;
		case 6: return canSheepMobSpawnHere(world,a,b); break;
		default: break;
	}
	return false;
}
void spawnMobOn(int mobId,worldObject* world,int j)
{
	int i;
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
					case 3: mobs[mobNum]= new zombieMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
					case 4: mobs[mobNum]= new pigMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
					case 5: mobs[mobNum]= new cowMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
					case 6: mobs[mobNum]= new sheepMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
					default: break;
				}
				mobs[mobNum]->host=true;
			}
			else printf("Can't find number\n");
			i=WORLD_HEIGHT+1;
			j=WORLD_WIDTH+1;
		}
}
int spawnMob(int mobId,worldObject* world)
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
					case 3: mobs[mobNum]= new zombieMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
					case 4: mobs[mobNum]= new pigMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
					case 5: mobs[mobNum]= new cowMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
					case 6: mobs[mobNum]= new sheepMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
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
			case 3: mobs[mobNum]= new zombieMob(x,y); mobs[mobNum]->unKillMob(); break;
			case 4: mobs[mobNum]= new pigMob(x,y); mobs[mobNum]->unKillMob(); break;
			case 5: mobs[mobNum]= new cowMob(x,y); mobs[mobNum]->unKillMob(); break;
			case 6: mobs[mobNum]= new sheepMob(x,y); mobs[mobNum]->unKillMob(); break;
			default: break;
		}
		mobs[mobNum]->host=true;
	}
}
void spawnMobNoCheck(int mobId,worldObject* world,int mobNum)
{
	delete mobs[mobNum];
	switch(mobId)
	{
		case 0: mobs[mobNum]= new baseMob(0,0); mobs[mobNum]->unKillMob(); break;
		case 1: mobs[mobNum]= new playerMob(0,0); mobs[mobNum]->unKillMob(); break;
		case 2: mobs[mobNum]= new MplayerMob(0,0); mobs[mobNum]->unKillMob(); break;
		case 3: mobs[mobNum]= new zombieMob(0,0); mobs[mobNum]->unKillMob(); break;
		case 4: mobs[mobNum]= new pigMob(0,0); mobs[mobNum]->unKillMob(); break;
		case 5: mobs[mobNum]= new cowMob(0,0); mobs[mobNum]->unKillMob(); break;
		case 6: mobs[mobNum]= new sheepMob(0,0); mobs[mobNum]->unKillMob(); break;
		default: break;
	}
	mobs[mobNum]->host=false;
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
				case 3: mobs[mobNum]= new zombieMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
				case 4: mobs[mobNum]= new pigMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
				case 5: mobs[mobNum]= new cowMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
				case 6: mobs[mobNum]= new sheepMob(j*16,i*16); mobs[mobNum]->unKillMob(); break;
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
		spawnMobNoCheck(mobtype,world,mobNum);
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
	int badMobs = 0;
	int goodMobs = 0;
	if (!hasSpawnPlayer)
	{
		spawnMob(1,world);
		hasSpawnPlayer = true;
	}
	int i;
	for(i=1;i<100;i++)
	{

		if (mobs[i]->alive == true)
		{
			if (mobs[i]->mobtype == 3)
				badMobs++;
			if (mobs[i]->mobtype == 4 || mobs[i]->mobtype == 5 || mobs[i]->mobtype == 6)
				goodMobs++;
			if (mobs[i]->smallmob == false) calculateMiscData(world,mobs[i]);
			else calculateMiscDataSmall(world,mobs[i]);
			mobs[i]->updateMob(world);
			mobs[i]->timeTillWifiUpdate--;
			if (isWifi())
			{
				mobs[i]->ping++;
				if(mobs[i]->ping>80)
				{
					mobs[i]->ping = 0;
					mobs[i]->alive = false;
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
	//if (keysDown() & KEY_Y) spawnMobOn(4,world,mobs[playerId]->x/16 + (rand() % 9 - 4));
	if (keysDown() & KEY_Y)
	{
		spawnMobOn(6,world,mobs[playerId]->x/16);
	}
	if (goodMobs<=20 && canSpawnMob() && rand() % 30 == 0)
	{
		int mob = rand() % 2;
		spawnMobOn(peacefulmobs[mob],world,rand() % WORLD_WIDTH);
	}
	if (badMobs<=2 && canSpawnMob())
	{	
		int take = 0;
		if (rand() % 2)
		{
			take =-16 -(rand () % 16);
		}
		else take = 16 + (rand() % 16);
		spawnMobOn(3,world,mobs[playerId]->x/16 + take);
	}
}
