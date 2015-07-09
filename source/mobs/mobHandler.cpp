#include <nds.h>
#include <fat.h>
#include <vector>
#include <memory>
#include "BaseMob.h" //Bad guys are nameMob
#include "PlayerMob.h" //Good guys are mobName
#include "MultiplayeMob.h"
#include "ZombieMob.h"
#include "HerobrineMob.h"
#include "AnimalMob.h"
#include "ItemMob.h"
#include "mobFunctions.h"
#include "hurt.h"
#include "../nifi.h"
#include "../communications.h"
#include "../collision.h"
#include "../general.h"
#include "../Config.h"
#include "../blockID.h"
#include "../blocks.h"
#include "../worldRender.h"
#define PLAYER_ID 1

std::vector<BaseMob_ptr> mobs;
//BaseMob* mobs[100];
bool hasSpawnedPlayer;
bool spawnPlayerAtPos;

void createItemMob(int x, int y, int blockID, int amount, int displayID, float initVX)
{
	if (amount < 1 || blockID == AIR)
		return;
	if (displayID == -1)
		displayID = blockID;
	mobs.push_back(BaseMob_ptr(new ItemMob(x * 16 + 4, y * 16,blockID, amount, displayID, initVX)));
}

bool canMobSpawnHere(WorldObject *world, int x, int y)
{
	return (isBlockWalkThrough(world->blocks[x][y]) && !isBlockWalkThrough(world->blocks[x][y + 1]) && world->blocks[x][y] != CACTUS && world->bgblocks[x][y + 1] != CACTUS);
}

int getDefaultSpawnX()
{
	return mobs[PLAYER_ID]->x / 16 + (rand() % 5) - 2;
}

BaseMob_ptr mobHandlerFindMob(int range, MobType type, int x, int y)
{
	int closest = range * range + 1;
	int index = -1;
	for (std::vector<BaseMob_ptr>::size_type i = 0; i != mobs.size(); ++i)
	{
		if (mobs[i]->type == type)
		{
			if (mobs[i]->x > x - range && mobs[i]->x < x + range)
				if (mobs[i]->y > y - range && mobs[i]->y < y + range)
				{
					int tmp = abs(mobs[i]->x - x) + abs(mobs[i]->y - y);
					if (tmp < closest)
					{
						closest = tmp;
						index = i;
					}
				}
		}
	}
	if (index != -1)
		return mobs[index];
	return NULL;
}

int getPlayerX()
{
	return mobs[PLAYER_ID]->x;
}

void mobHandlerHurtMobWifi(int index, int amount, int type)
{
	if (mobs[index]->host == true)
		mobs[index]->hurt(amount, type);
}

BaseMob_ptr isMobAt(int x, int y)
{
	for (std::vector<BaseMob_ptr>::size_type i = 0; i != mobs.size(); ++i)
		if (spriteCol(mobs[i]->x - mobs[i]->sx / 2 + 1, mobs[i]->y - mobs[i]->sy / 2 + 1, x - 1, y - 1, mobs[i]->sx, mobs[i]->sy, 1, 1) && mobs[i]->alive == true)
			return mobs[i];
	return NULL;
}

void mobsReset(bool playerSpawned)
{
	mobs.clear();
	hasSpawnedPlayer = playerSpawned;
	spawnPlayerAtPos = playerSpawned;
}

void mobHandlerInit()
{
	playerMobInit();
	multiplayerMobInit();
	zombieMobInit();
	herobrineMobInit();
	animalMobInit();
	hasSpawnedPlayer = false;
	spawnPlayerAtPos = false;
}

static bool canMobSpawnHere(MobType type, WorldObject* world, int a, int b)
{
	switch (type)
	{
		case MOB_PLAYER:
			return canPlayerMobSpawnHere(world, a, b);
		case MOB_MULTIPLAYER:
			return canMultiplayerMobSpawnHere(world, a, b);
		case MOB_ZOMBIE:
			return canZombieMobSpawnHere(world, a, b);
		case MOB_ANIMAL:
			return canAnimalMobSpawnHere(world, a, b);
		case MOB_HEROBRINE:
			return canHerobrineMobSpawnHere(world, a, b);
		default:
			showError("Checking spawn for non-existent mob type");
			break;
	}
	return false;
}

static void newMob(MobType type, int x = 0, int y = 0)
{
	switch (type)
	{
		case MOB_PLAYER:
			mobs.push_back(BaseMob_ptr(new PlayerMob(x, y)));
			break;
		case MOB_MULTIPLAYER:
			mobs.push_back(BaseMob_ptr(new MultiplayerMob(x, y)));
			break;
		case MOB_ZOMBIE:
			mobs.push_back(BaseMob_ptr(new ZombieMob(x, y)));
			break;
		case MOB_ANIMAL:
			mobs.push_back(BaseMob_ptr(new AnimalMob(x, y)));
			break;
		case MOB_HEROBRINE:
			mobs.push_back(BaseMob_ptr(new HerobrineMob(x, y)));
			break;
		default:
			showError("Unknown Mob Spawned");
			break;
	}
	mobs.back()->revive();
	mobs.back()->x += mobs.back()->sx / 2;
	mobs.back()->y += mobs.back()->sy / 2;
}

void saveMobs(FILE* f)
{
	for (std::vector<BaseMob_ptr>::size_type i = 0; i != mobs.size(); ++i)
	{
		if (mobs[i]->alive)
		{
			fprintf(f, "%d ", mobs[i]->type);
			mobs[i]->saveToFile(f);
		}
	}
	fprintf(f, "-1 ");
}

static void spawnMobOn(MobType mobId, WorldObject* world, int j, bool skipCheck = false)
{
	int i;
	for (i = 0; i <= WORLD_HEIGHT; ++i)
		if (canMobSpawnHere(mobId, world, j, i) || skipCheck)
		{
			newMob(mobId, j * 16, i * 16);
			mobs.back()->host = true;
			i = WORLD_HEIGHT + 1;
			j = WORLD_WIDTH + 1;
		}
}

static int spawnMob(MobType mobId, WorldObject* world)
{
	int i;
	int j;
	for (j = 0; j <= WORLD_WIDTH; ++j)
		for (i = 0; i <= WORLD_HEIGHT; ++i)
			if (canMobSpawnHere(mobId, world, j, i))
			{
				newMob(mobId, j * 16, i * 16);
				mobs.back()->host = true;
				i = WORLD_HEIGHT + 1;
				j = WORLD_WIDTH + 1;
			}
	return -1;
}

int spawnMobAt(MobType type, int x, int y)
{
	newMob(type, x, y);
	mobs.back()->host = true;
	return mobs.size()-1;
}

void loadMobs(FILE* f)
{
	mobsReset(false);
	int index = fscanf(f, "%d ", &index);
	while (index != -1)
	{
		if (index == MOB_PLAYER)
			hasSpawnedPlayer = true;
		mobs[spawnMobAt((MobType) index, 0, 0)]->loadFromFile(f);
		fscanf(f, "%d ", &index);
	}
}

static void spawnMobNoCheck(MobType type, WorldObject* world)
{
	newMob(type);
	mobs.back()->host = false;
}

void mobHandlerReadWifiUpdate(int x, int y, int animation, MobType type, int index, WorldObject* world, bool facing)
{
	//printf("Recieved mob update! - %d, %d\n", index,mobtype);
	if (mobs[index]->type != type)
	{
		if (mobs[index]->type == MOB_PLAYER)
			spawnMobAt(MOB_PLAYER, mobs[index]->x, mobs[index]->y);
		spawnMobNoCheck(type, world);
	}
	mobs[index]->revive();
	mobs[index]->x = x;
	mobs[index]->y = y;
	mobs[index]->spriteState = animation;
	mobs[index]->host = false;
	mobs[index]->facing = facing;
	mobs[index]->ping = 0;
	//:D
}

void mobHandlerUpdate(WorldObject* world)
{
	int badMobs = 0;
	int goodMobs = 0;
	if (!hasSpawnedPlayer)
	{
		spawnMob(MOB_PLAYER, world);
		hasSpawnedPlayer = true;
	}
	for (std::vector<BaseMob_ptr>::size_type i = 0; i < mobs.size(); ++i)
	{

		if (mobs[i]->health>0)
		{
			if (mobs[i]->type == MOB_ZOMBIE || mobs[i]->type == MOB_HEROBRINE)
				++badMobs;
			else if (mobs[i]->type == MOB_ANIMAL)
				++goodMobs;
			if (mobs[i]->smallmob == false && mobs[i]->type != MOB_ITEM) calculateMiscData(world, mobs[i]);
			else if (mobs[i]->type != MOB_ITEM) calculateMiscDataSmall(world, mobs[i]);
			mobs[i]->updateMob(world);
			--mobs[i]->timeTillWifiUpdate;
			if (isWifi())
			{
				++mobs[i]->ping;
				if (mobs[i]->ping > 80)
				{
					mobs[i]->ping = 0;
					mobs[i]->alive = false;
					mobs.erase(mobs.begin() + i);
				}
			}
		}
		else
		{
			mobs[i]->kill();
			mobs.erase(mobs.begin()+i);
			continue;
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
		spawnMobOn(MOB_ANIMAL, world, rand() % WORLD_WIDTH);
	if (badMobs <= 5 && canSpawnMob())
	{
		int take = 0;
		if (rand() % 2)
			take = -16 - (rand() % 16);
		else take = 16 + (rand() % 16);
		spawnMobOn((rand() % 10) != 1 && getGlobalSettings()->getProperty(PROPERTY_HEROBRINE) ? MOB_HEROBRINE : MOB_ZOMBIE, world, mobs[PLAYER_ID]->x / 16 + take);
	}
}
