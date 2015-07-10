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
private:
	virtual int getMaxHealth()
	{
		return 10;
	}
public:
	MobType type;
	int health; //0 = Dead
	float x, y, vx, vy;
	int sx, sy;
	bool smallMob, alive, onCactus, facing, collisions[5];
	int spriteState, framesHurtSprite, timeOnCactus;

	//Multiplayer
	bool host;

	virtual void updateMob(WorldObject* world) = 0;
	virtual void calcMiscData(WorldObject *world) = 0;
	virtual void saveToFile(FILE* sFile) = 0;
	virtual void loadFromFile(FILE* sFile) = 0;
	virtual void hurt(int amount, int type);
	virtual bool isMyPlayer();

	BaseMob(MobType type, int x, int y, int sx, int sy) :
	type(type), x(x + sx / 2), y(y + sy / 2), vx(0), vy(0), sx(sx), sy(sy),
	onCactus(false), facing(true), collisions { }, spriteState(0), framesHurtSprite(0),
	timeOnCactus(30), host(true)
	{
		health = getMaxHealth();
	}
	virtual ~ BaseMob() { }
};
typedef std::shared_ptr<BaseMob> BaseMob_ptr;
void BaseMobInit();
bool jumpHurtType(int hurtType);