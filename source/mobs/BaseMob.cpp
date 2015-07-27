#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "BaseMob.h"
#include "../blockID.h"
#include "hurt.h"
#include "../general.h"

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