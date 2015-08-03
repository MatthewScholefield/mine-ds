#pragma once
#include <stdio.h>
#include <memory>
#include "../world.h"
#include "../graphics/graphics.h"
#include "../FixedPoint.h"

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
protected:
	Graphic normalSprite, hurtSprite;
public:
	MobType type;
	int health; //0 = Dead
	FixedPoint x, y, vx, vy;
	int sx, sy;
	bool smallMob, alive, onCactus, facing, collisions[5];
	int spriteState, framesHurtSprite, timeOnCactus, framesFarAway, brightness;

	//Multiplayer
	bool host;

	virtual void updateMob(WorldObject* world) = 0;
	virtual void calcMiscData(WorldObject *world) = 0;
	virtual void saveToFile(FILE* sFile) = 0;
	virtual void loadFromFile(FILE* sFile) = 0;
	virtual void hurt(int amount, int type);
	virtual void calcHealth();
	virtual bool isMyPlayer();
	bool canJump(WorldObject *world = nullptr);

	BaseMob(MobType type, int x, int y, int sx, int sy) :
	normalSprite(), hurtSprite(), type(type), health(256), x(x + sx / 2),
	y(y + sy / 2), vx(0), vy(0), sx(sx), sy(sy), smallMob(false),
	alive(true), onCactus(false), facing(true), collisions { }

	,
	spriteState(0), framesHurtSprite(0),
	timeOnCactus(30), framesFarAway(0), brightness(-1), host(true) { }

	virtual ~BaseMob()
	{
		unloadGraphic(&normalSprite);
		unloadGraphic(&hurtSprite);
	}
};
typedef std::shared_ptr<BaseMob> BaseMob_ptr;
bool jumpHurtType(int hurtType);