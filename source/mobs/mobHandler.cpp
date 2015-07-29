#include <nds.h>
#include <fat.h>
#include <vector>
#include <memory>
#include "BaseMob.h" //Bad guys are nameMob
#include "PlayerMob.h" //Good guys are mobName
#include "MultiplayerMob.h"
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
#include "../deathScreen.h"
#include "../mainGame.h"
#define PLAYER_ID 1

std::vector<BaseMob_ptr> mobs;
bool hasSpawnedPlayer;

void createItemMob(int x, int y, int blockID, int amount, int displayID, float initVX)
{
	if (amount < 1 || blockID == AIR)
		return;
	if (displayID == -1)
		displayID = blockID;
	mobs.push_back(BaseMob_ptr(new ItemMob(x * 16 + 4, y * 16 - 4, blockID, amount, displayID, initVX)));
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
	return nullptr;
}

int getPlayerX()
{
	return mobs[PLAYER_ID]->x;
}

BaseMob_ptr isMobAt(int x, int y)
{
	for (std::vector<BaseMob_ptr>::size_type i = 0; i != mobs.size(); ++i)
		if (spriteCol(mobs[i]->x - mobs[i]->sx / 2 + 1, mobs[i]->y - mobs[i]->sy / 2 + 1, x - 1, y - 1, mobs[i]->sx, mobs[i]->sy, 1, 1) && mobs[i]->alive == true)
			return mobs[i];
	return nullptr;
}

void mobsReset(bool playerSpawned)
{
	mobs.clear();
	hasSpawnedPlayer = playerSpawned;
}

void mobHandlerInit()
{
	playerMobInit();
	hasSpawnedPlayer = false;
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
			return;
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
	return mobs.size() - 1;
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

void mobHandlerUpdate(WorldObject* world, touchPosition *touch)
{
	bool delTouch = false;
	if (!touch)
	{
		delTouch = true;
		touch = new touchPosition();
	}
	const int EXTRA = 128;
	int badMobs = 0;
	int goodMobs = 0;
	switch (deathScreenUpdate(touch))
	{
	case 0: //Respawn
		spawnMob(MOB_PLAYER, world);
		break;
	case 1: //Titlescreen
		quitGame();
		break;
	default: //Nothing
		break;
	}
	if (!hasSpawnedPlayer)
	{
		spawnMob(MOB_PLAYER, world);
		hasSpawnedPlayer = true;
	}
	for (std::vector<BaseMob_ptr>::size_type i = 0; i < mobs.size(); ++i)
	{
		if (mobs[i]->health > 0)
		{
			mobs[i]->calcHealth();
			bool closeToPlayer = !(mobs[i]->x < world->camX - EXTRA || mobs[i]->x > world->camX + 256 + EXTRA
					|| mobs[i]->y < world->camY - EXTRA || mobs[i]->y > world->camY + 192 + EXTRA);
			if (mobs[i]->type == MOB_ZOMBIE || mobs[i]->type == MOB_HEROBRINE)
				++badMobs;
			else if (mobs[i]->type == MOB_ANIMAL)
				++goodMobs;
			if (closeToPlayer || mobs[i]->type == MOB_PLAYER)
			{
				mobs[i]->framesFarAway = 0;
				mobs[i]->calcMiscData(world);
				mobs[i]->updateMob(world);
			}
			else if (!closeToPlayer)
			{
				if (mobs[i]->framesFarAway > SEC_TO_FPS(6))
				{
					mobs.erase(mobs.begin() + i);
					continue;
				}
				else
					++mobs[i]->framesFarAway;
			}
		}
		else
		{
			mobs.erase(mobs.begin() + i);
			continue;
		}
	}
	if (goodMobs < 3)// && rand() % 30 == 0)
		spawnMobOn(MOB_ANIMAL, world, world->camX / 16 + rand() % 16);
	if (badMobs <= 5 && canSpawnMob())
		spawnMobOn((rand() % 10) != 1
				&& getGlobalSettings()->getProperty(PROPERTY_HEROBRINE) ? MOB_HEROBRINE : MOB_ZOMBIE,
				world, mobs[PLAYER_ID]->x / 16 + (16 + (rand() % 16))*((rand() % 2) ? -1 : 1));
	if (delTouch)
		delete touch;
}
