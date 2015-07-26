#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "BaseMob.h"
#include "../blockID.h"
#include "hurt.h"
#include "../general.h"

/*BaseMob::BaseMob()
{
	host = true;
	facing = false;
	for (int i = 0; i < 5; ++i)
		collisions[i] = false;
	timeTillWifiUpdate = rand() % 4 + 4;
	spriteState = 0;
	sx = 6;
	sy = 32;
	timeOnCactus = 30;
	onCactus = false;
	x = 0;
	y = 0;
	vy = 0;
	vx = 0;
	ping = 0;
	alive = false;
	health = 10;
	framesHurtSprite = 0;

}

BaseMob::BaseMob(int a, int b)
{
	host = true;
	for (int i = 0; i < 5; ++i)
		collisions[i] = false;
	spriteState = 0;
	onCactus = false;
	timeOnCactus = 30;
	sx = 6;
	sy = 32;
	x = a;
	y = b;
	vy = 0;
	vx = 0;
	alive = false;
	facing = false;
	ping = 0;
	health = 10;
	framesHurtSprite = 0;
	timeTillWifiUpdate = rand() % 4 + 4;
}*/

bool canBaseMobSpawnHere(WorldObject* world, int x, int y)
{
	++y;
	if (!isBlockWalkThrough(world->blocks[x][y + 1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y] != CACTUS && world->blocks[x][y + 1] != CACTUS) return true;
	return false;
}

void BaseMob::calcHealth()
{
	if (health>getMaxHealth())
		health=getMaxHealth();
	else if (health<getMaxHealth() && getTime()%256==0)
		++health;
		
}

void BaseMob::hurt(int amount, int type)
{
	if (!host || health < 0)
		return;
	health -= amount;
	spriteState = 1;
	framesHurtSprite = 20;
}

bool BaseMob::isMyPlayer()
{
	return false;
}

bool jumpHurtType(int hurtType)
{
	switch (hurtType)
	{
	case VOID_HURT:
	case FALL_HURT:
		return false;
	default:
		return true;
	}
}