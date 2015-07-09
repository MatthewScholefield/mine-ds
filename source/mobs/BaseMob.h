#pragma once
#include <stdio.h>
#include <memory>
#include "../world.h"

#define SIDE_BOTTOM 0
#define SIDE_RIGHT 1
#define SIDE_LEFT 2
#define SIDE_TOP 3
#define COLLISION_STUCK 4

enum MobType
{
	MOB_ANIMAL = 0,
	MOB_HEROBRINE = 1,
	MOB_ITEM = 2,
	MOB_MULTIPLAYER = 3,
	MOB_PLAYER = 4,
	MOB_ZOMBIE = 5,
};

bool canBaseMobSpawnHere(WorldObject* world, int x, int y);

class BaseMob
{
public:
	float x, y;
	float vy, vx;
	int sx, sy;
	bool smallmob, alive;
	int spriteState, framesHurtSprite;
	MobType type;
	int timeOnCactus;
	bool onCactus, facing;
	int health; //0 = Dead

	//Multiplayer
	int timeTillWifiUpdate;
	int ping;
	bool host;
	bool collisions[5];

	virtual void updateMob(WorldObject* world) = 0;
	virtual void saveToFile(FILE* sFile) = 0;
	virtual void loadFromFile(FILE* sFile) = 0;
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	virtual void kill();
	virtual void revive();

	BaseMob()
	{
		host = true;
		facing = false;
		for (int i = 0; i < 5; ++ i)
			collisions[i] = false;
		timeTillWifiUpdate = 0;
		spriteState = 0;
		timeOnCactus = 30;
		onCactus = false;
		ping = 0;
		alive = true;
		health = 10;
		framesHurtSprite = 0;
	}
	virtual ~ BaseMob() { }
};
typedef std::shared_ptr<BaseMob> BaseMob_ptr;
void BaseMobInit();
bool jumpHurtType(int hurtType);