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
#include "../world/worldRender.h"
#include "../deathScreen.h"
#include "../mainGame.h"
#include "WaterMob.h"
#include "mobHandler.h"
#include "../graphics/interfaces/Interface.h"
#include "../graphics/interfaces/interfaceHandler.h"

std::vector<BaseMob::Ptr> mobs;
BaseMob::Ptr playerPointer;
bool shouldSpawnPlayer = false;

void createWaterMob(int x, int y, int level)
{
	mobs.push_back(BaseMob::Ptr(new WaterMob(x * 16, y * 16 + 16, level)));
}

void createItemMob(int x, int y, int blockID, int amount, int displayID, float initVX)
{
	if (amount < 1 || blockID == AIR)
		return;
	if (displayID == -1)
		displayID = blockID;
	mobs.push_back(BaseMob::Ptr(new ItemMob(x * 16 + 7, y * 16 + 8, blockID, amount, displayID, initVX)));
}

bool canMobSpawnHere(World &world, int x, int y)
{
	return isBlockWalkThrough(world.blocks[x][y]) && !isBlockWalkThrough(world.blocks[x][y + 1]);
}

int getDefaultSpawnX()
{
	return !playerPointer ? 0 : (int) (playerPointer->x / 16 + (rand() % 5) - 2);
}

BaseMob::Ptr mobHandlerFindMob(int range, MobType type, int x, int y)
{
	int closest = range * range + 1;
	int index = -1;
	for (std::vector<BaseMob::Ptr>::size_type i = 0; i != mobs.size(); ++i)
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
	return !playerPointer ? 0 : (int) playerPointer->x;
}

BaseMob::Ptr getPlayerPtr()
{
	return playerPointer;
}

BaseMob::Ptr isMobAt(int x, int y)
{
	for (std::vector<BaseMob::Ptr>::size_type i = 0; i != mobs.size(); ++i)
		if (spriteCol(mobs[i]->x - mobs[i]->sx / 2 + 1, mobs[i]->y - mobs[i]->sy / 2 + 1, x - 1, y - 1, mobs[i]->sx, mobs[i]->sy, 1, 1) && mobs[i]->alive == true)
			return mobs[i];
	return nullptr;
}

void mobsReset()
{
	playerPointer.reset();
	mobs.clear();
	shouldSpawnPlayer = true;
}

void mobHandlerInit()
{
	playerMobInit();
	shouldSpawnPlayer = true;
}

static bool canMobSpawnHere(MobType type, World &world, int a, int b)
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
	case MOB_ITEM:
		return true;
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
		mobs.push_back(BaseMob::Ptr(new PlayerMob(x, y)));
		playerPointer = mobs.back();
		shouldSpawnPlayer = false;
		break;
	case MOB_MULTIPLAYER:
		mobs.push_back(BaseMob::Ptr(new MultiplayerMob(x, y)));
		break;
	case MOB_ZOMBIE:
		mobs.push_back(BaseMob::Ptr(new ZombieMob(x, y)));
		break;
	case MOB_ANIMAL:
		mobs.push_back(BaseMob::Ptr(new AnimalMob(x, y)));
		break;
	case MOB_HEROBRINE:
		mobs.push_back(BaseMob::Ptr(new HerobrineMob(x, y)));
		break;
	case MOB_ITEM:
		createItemMob(x, y, DIRT);
		break;
	default:
		showError("Unknown Mob Spawned");
		break;
	}
}

static void spawnMobOn(MobType mobId, World &world, int j, bool skipCheck = false)
{
	int i;
	for (i = 0; i < World::HEIGHT; ++i)
		if (canMobSpawnHere(mobId, world, j, i) || skipCheck)
		{
			newMob(mobId, (World::BLOCK_PX * (2 * j + 1)) / 2, (i + 1) * 16);
			mobs.back()->y -= mobs.back()->sy / 2;
			mobs.back()->host = true;
			return;
		}
}

static void createMobAtSpawn(MobType mobId, World &world)
{
	for (int j = world.spawnX; j < World::WIDTH; ++j)
		for (int i = 0; i < World::HEIGHT; ++i)
			if (canMobSpawnHere(mobId, world, j, i))
			{
				newMob(mobId, j * 16, i * 16);
				mobs.back()->host = true;
				return;
			}
}

int spawnMobAt(MobType type, int x, int y)
{
	newMob(type, x, y);
	mobs.back()->host = true;
	return mobs.size() - 1;
}

void saveMobs(FILE* f)
{
	for (std::vector<BaseMob::Ptr>::size_type i = 0; i != mobs.size(); ++i)
	{
		if (mobs[i]->alive)
		{
			fprintf(f, "%d ", mobs[i]->type);
			mobs[i]->saveToFile(f);
		}
	}
	fprintf(f, "-1 ");
}


void loadMobs(FILE* f)
{
	mobsReset();
	int index = 0;
	do
	{
		fscanf(f, "%d ", &index);
		if (index >= 0 && spawnMobAt((MobType) index, 0, 0) >= 0)
			mobs.back()->loadFromFile(f);
	}
	while (index >= 0);
}

void triggerPlayerRespawn()
{
	shouldSpawnPlayer = true;
}

void mobHandlerUpdate(World &world, touchPosition &touch)
{
	const int EXTRA = 128;
	int badMobs = 0;
	int goodMobs = 0;
	if (shouldSpawnPlayer)
		createMobAtSpawn(MOB_PLAYER, world);
	for (std::vector<BaseMob::Ptr>::size_type i = 0; i < mobs.size(); ++i)
	{
		if (mobs[i]->health > 0)
		{
			mobs[i]->calcHealth();
			bool closeToPlayer = !(mobs[i]->x < world.camX - EXTRA || mobs[i]->x > world.camX + 256 + EXTRA
					|| mobs[i]->y < world.camY - EXTRA || mobs[i]->y > world.camY + 192 + EXTRA);
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
				if (mobs[i]->framesFarAway > SEC_TO_FPS(6) && mobs[i]->type != MOB_ITEM)
				{
					mobs.erase(mobs.begin() + i);
					--i;
					continue;
				}
				else
					++mobs[i]->framesFarAway;
			}
		}
		else
		{
			if (mobs[i]->isMyPlayer())
				setInterface(world, INTERFACE_DEATH_SCREEN);
			mobs.erase(mobs.begin() + i);
			--i;
			continue;
		}
	}
	if (goodMobs < 4 && rand() % 30 == 0)
		spawnMobOn(MOB_ANIMAL, world, world.camX / 16 + rand() % 16);
	if (badMobs <= 5)
		spawnMobOn((rand() % 10) != 1
				&& getGlobalSettings()->getProperty(PROPERTY_HEROBRINE) ? MOB_HEROBRINE : MOB_ZOMBIE,
			world, getPlayerX() / 16 + (16 + (rand() % 16))*((rand() % 2) ? -1 : 1));
}
