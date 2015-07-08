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
	float x;
	float y;
	float vy;
	float vx;
	int sx;
	int sy;
	bool smallmob = false;
	bool alive;
	int spriteState;
	int framesHurtSprite;
	MobType mobType;
	int timeOnCactus;
	bool onCactus;
	int health; //Health of the mob, 0 is dead ;)
	int timeTillWifiUpdate;
	int ping;
	int mobId; // Needed for chasing mobs
	bool host; // Was this mob spawn'd by this nds?
	bool collisions[5];
	//bool potioneffects[5];
	virtual void saveToFile(FILE* sFile) = 0;
	virtual void loadFromFile(FILE* sFile) = 0;
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();
	bool facing;
	void killMob();
	void unKillMob();
	void setXYPos(int x, int y);
	void resetVelocity();
	virtual void updateMob(WorldObject* world);
	BaseMob();
	BaseMob(int x, int y);

	virtual ~ BaseMob() { }
};
typedef std::shared_ptr<BaseMob> BaseMob_ptr;
void BaseMobInit();
bool jumpHurtType(int hurtType);