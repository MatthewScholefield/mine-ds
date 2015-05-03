#include <nds.h>
#include <fat.h>
#include "baseMob.h" //Bad guys are nameMob
#include "mobPlayer.h" //Good guys are mobName
#include "mobMPlayer.h"
#include "zombieMob.h"
#include "herobrineMob.h"
#include "pigMob.h"
#include "cowMob.h"
#include "sheepMob.h"
#include "itemMob.h"
#include "mobFunctions.h"
#include "hurt.h"
#include "../nifi.h"
#include "../communications.h"
#include "../collision.h"
#include "../general.h"
#include "../Config.h"
#include "../blockID.h"
#include "../blocks.h"
#define playerId 1

baseMob* mobs[100];
const int peacefulmobs[] = {4, 5, 6};
bool hasSpawnPlayer;
bool spawnPlayerAtPos;
int spawn_x, spawn_y;

bool canMobSpawnHere(worldObject *world, int x, int y)
{
	return (isBlockWalkThrough(world->blocks[x][y]) && !isBlockWalkThrough(world->blocks[x][y + 1]) && world->blocks[x][y] != CACTUS && world->bgblocks[x][y + 1] != CACTUS);
}

int getDefaultSpawnX()
{
	return mobs[playerId]->x / 16 + (rand() % 5) - 2;
}

baseMob* mobHandlerFindMob(int range, int type, int x, int y)
{
	int closest = range * range + 1;
	int mobNum = -1;
	int i;
	for (i = 0; i < 100; ++i)
	{
		if (mobs[i]->mobtype == type)
		{
			if (mobs[i]->x > x - range && mobs[i]->x < x + range)
				if (mobs[i]->y > y - range && mobs[i]->y < y + range)
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
		mobs[mobNum]->mobId = mobNum;
		return mobs[mobNum];
	}
	return NULL;
}

void mobHandlerHurtMobWifi(int mobNum, int amount, int type)
{
	if (mobs[mobNum]->host == true)
		mobs[mobNum]->hurt(amount, type);
}

void mobHandlerHurtMob(int mobNum, int amount, int type)
{
	if (mobs[mobNum]->mobtype == 8 && type != PROPERTY_HURT) //itemMob
		return;
	if (mobs[mobNum]->host == true && mobs[mobNum]->health > 0)
		mobs[mobNum]->hurt(amount, type);
	else if (isWifi())
		wifiHurtMob(mobNum, amount, type);
}

int isMobAt(int x, int y)
{
	int i;
	for (i = 0; i <= 100; ++i)
	{
		if (spriteCol(mobs[i]->x - mobs[i]->sx / 2 + 1, mobs[i]->y - mobs[i]->sy / 2 + 1, x - 1, y - 1, mobs[i]->sx, mobs[i]->sy, 1, 1) && mobs[i]->alive == true)
			return i;
	}
	return -1;
}

void mobHandlerKillMob(int mobNum)
{
	mobs[mobNum]->killMob();
}

void mobsReset(bool playerSpawned)
{
	int i;
	for (i = 0; i < 100; ++i)
	{
		delete mobs[i];
		mobs[i] = new baseMob();
		mobs[i] -> killMob();
	}
	hasSpawnPlayer = playerSpawned;
	spawnPlayerAtPos = playerSpawned;
}

void mobHandlerInit()
{
	baseMobInit();
	playerMobInit();
	MplayerMobInit();
	zombieMobInit();
	herobrineMobInit();
	pigMobInit();
	cowMobInit();
	sheepMobInit();
	int i;
	for (i = 0; i < 100; ++i)
	{
		mobs[i] = new baseMob();
		mobs[i] -> killMob();
	}
	hasSpawnPlayer = false;
	spawnPlayerAtPos = false;
}

int findFreeMobSpawnNum()
{
	int i;
	for (i = 1; i <= 100; ++i)
		if (mobs[i]->alive == false && mobs[i]->isMyPlayer() == false)
			return i;
	return -1;
}

bool canMobSpawnHere(int mobId, worldObject* world, int a, int b)
{
	switch (mobId)
	{
		case 0:
			return canBaseMobSpawnHere(world, a, b);
			break;
		case 1:
			return canPlayerMobSpawnHere(world, a, b);
			break;
		case 2:
			return canPlayerMobSpawnHere(world, a, b);
			break;
		case 3:
			return canZombieMobSpawnHere(world, a, b);
			break;
		case 4:
			return canPigMobSpawnHere(world, a, b);
			break;
		case 5:
			return canCowMobSpawnHere(world, a, b);
			break;
		case 6:
			return canSheepMobSpawnHere(world, a, b);
			break;
		case 7:
			return canHerobrineMobSpawnHere(world, a, b);
			break;
		default:
			break;
	}
	return false;
}

void newMob(int mobId, int mobNum, int x = 0, int y = 0)
{
	delete mobs[mobNum]; //Free Memory and Stop Crashes
	switch (mobId)
	{
		case 0:
			mobs[mobNum] = new baseMob(x, y);
			mobs[mobNum]->unKillMob();
			break;
		case 1:
			mobs[mobNum] = new playerMob(x, y);
			mobs[mobNum]->unKillMob();
			break;
		case 2:
			mobs[mobNum] = new MplayerMob(x, y);
			mobs[mobNum]->unKillMob();
			break;
		case 3:
			mobs[mobNum] = new zombieMob(x, y);
			mobs[mobNum]->unKillMob();
			break;
		case 4:
			mobs[mobNum] = new pigMob(x, y);
			mobs[mobNum]->unKillMob();
			break;
		case 5:
			mobs[mobNum] = new cowMob(x, y);
			mobs[mobNum]->unKillMob();
			break;
		case 6:
			mobs[mobNum] = new sheepMob(x, y);
			mobs[mobNum]->unKillMob();
			break;
		case 7:
			mobs[mobNum] = new herobrineMob(x, y);
			mobs[mobNum]->unKillMob();
			break;
		case 8:
			mobs[mobNum] = new itemMob(x, y);
			mobs[mobNum]->unKillMob();
		default:
			break;
	}
}

void saveMobs(FILE* f)
{
	int i;
	for (i = 0; i < 100; ++i)
	{
		if (mobs[i]->alive)
		{
			int mobType;
			mobType = mobs[i]->mobtype;
			if (mobType == 2 && mobs[i]->isMyPlayer()) mobType = 1;
			fprintf(f, "%d ", mobType);
			mobs[i]->saveToFile(f);
		}
	}
	fprintf(f, " -1");
}

void spawnMobOn(int mobId, worldObject* world, int j, bool skipCheck = false)
{
	int i;
	for (i = 0; i <= WORLD_HEIGHT; ++i)
		if (canMobSpawnHere(mobId, world, j, i) || skipCheck)
		{
			int mobNum = findFreeMobSpawnNum();
			if (mobNum != -1)
			{
				newMob(mobId, mobNum, j * 16, i * 16);
				mobs[mobNum]->host = true;
			}
			else printf("Can't find number\n");
			i = WORLD_HEIGHT + 1;
			j = WORLD_WIDTH + 1;
		}
}

int spawnMob(int mobId, worldObject* world)
{
	int i;
	int j;
	for (j = 0; j <= WORLD_WIDTH; ++j)
		for (i = 0; i <= WORLD_HEIGHT; ++i)
			if (canMobSpawnHere(mobId, world, j, i))
			{
				int mobNum = findFreeMobSpawnNum();
				if (mobNum >= 0)
				{
					newMob(mobId, mobNum, j * 16, i * 16);
					mobs[mobNum]->host = true;
				}
				else printf("Can't find number\n");
				i = WORLD_HEIGHT + 1;
				j = WORLD_WIDTH + 1;
			}
	return -1;
}

int spawnMobAt(int mobId, worldObject* world, int x, int y)
{
	int mobNum = findFreeMobSpawnNum();
	if (mobNum >= 0)
	{
		newMob(mobId, mobNum, x, y);
		mobs[mobNum]->host = true;
	}
	return mobNum;
}

void loadMobs(FILE* f)
{
	mobsReset(false);
	int shouldContinue = true;
	while (shouldContinue)
	{
		int mobType;
		fscanf(f, "%d ", &mobType);
		if (mobType != -1)
		{
			int mobId;
			mobId = spawnMobAt(mobType, NULL, 0, 0);
			if (mobType == 1)
			{
				hasSpawnPlayer = true;
			}
			mobs[mobId]->loadFromFile(f);
		}
		else
		{
			shouldContinue = false;
		}
	}
}

void spawnMobNoCheck(int mobId, worldObject* world, int mobNum)
{
	if (mobNum >= 0)
	{
		newMob(mobId, mobNum);
		mobs[mobNum]->host = false;
	}
}

void spawnMob(int mobId, worldObject* world, int mobNum)
{
	int i;
	int j;
	for (j = 0; j <= WORLD_WIDTH; ++j)
		for (i = 0; i <= WORLD_HEIGHT; ++i)
			if (canMobSpawnHere(mobId, world, j, i))
			{
				if (mobNum >= 0)
				{
					newMob(mobId, mobNum, j * 16, i * 16);
					mobs[mobNum]->host = false;
				}
				i = WORLD_HEIGHT + 1;
				j = WORLD_WIDTH + 1;
			}
}

void mobHandlerReadWifiUpdate(int x, int y, int animation, int mobtype, int mobNum, worldObject* world, bool facing)
{
	//printf("Recieved mob update! - %d, %d\n", mobNum,mobtype);
	if (mobs[mobNum]->mobtype != mobtype)
	{
		if (mobs[mobNum]->mobtype == 1)
			spawnMobAt(1, world, mobs[mobNum]->x, mobs[mobNum]->y);
		spawnMobNoCheck(mobtype, world, mobNum);
	}
	mobs[mobNum]->unKillMob();
	mobs[mobNum]->x = x;
	mobs[mobNum]->y = y;
	mobs[mobNum]->animation = animation;
	mobs[mobNum]->host = false;
	mobs[mobNum]->facing = facing;
	mobs[mobNum]->ping = 0;
	//:D
}

void mobHandlerUpdate(worldObject* world)
{
	int badMobs = 0;
	int goodMobs = 0;
	if (!hasSpawnPlayer)
	{
		spawnMob(1, world);
		hasSpawnPlayer = true;
	}
	int i;
	for (i = 1; i < 100; ++i)
	{

		if (mobs[i]->alive == true)
		{
			if (mobs[i]->mobtype == 3 || mobs[i]->mobtype == 7)
				++badMobs;
			if (mobs[i]->mobtype == 4 || mobs[i]->mobtype == 5 || mobs[i]->mobtype == 6)
				++goodMobs;
			if (mobs[i]->smallmob == false) calculateMiscData(world, mobs[i]);
			else calculateMiscDataSmall(world, mobs[i]);
			mobs[i]->updateMob(world);
			--mobs[i]->timeTillWifiUpdate;
			if (isWifi())
			{
				++mobs[i]->ping;
				if (mobs[i]->ping > 80)
				{
					mobs[i]->ping = 0;
					mobs[i]->alive = false;
					delete mobs[i];
				}
			}
		}
		if (mobs[i]->timeTillWifiUpdate == 0 && isWifi())
		{

			if (mobs[i]->host == true)
			{
				sendMobUpdater(mobs[i], i);
				mobs[i]->timeTillWifiUpdate = rand() % 3 + 3;

			}

		}
		else if (mobs[i]->timeTillWifiUpdate == 0) mobs[i]->timeTillWifiUpdate = 255;
	}
	if (goodMobs <= 20 && canSpawnMob() && rand() % 30 == 0)
	{
		int mob = rand() % 3;
		spawnMobOn(peacefulmobs[mob], world, rand() % WORLD_WIDTH);
	}
	if (badMobs <= 5 && canSpawnMob())
	{
		int take = 0;
		if (rand() % 2)
			take = -16 - (rand() % 16);
		else take = 16 + (rand() % 16);
		spawnMobOn((rand() % 10) != 1 && getGlobalSettings()->getProperty(PROPERTY_HEROBRINE) ? 7 : 3, world, mobs[playerId]->x / 16 + take);
	}
}
