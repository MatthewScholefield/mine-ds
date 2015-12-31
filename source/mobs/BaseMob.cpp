#include <stdio.h>
#include "../world/World.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "BaseMob.h"
#include "../blockID.h"
#include "hurt.h"
#include "../general.h"
#include "../Config.h"
#include "../blockUpdaters/WaterUpdater.h"

bool canBaseMobSpawnHere(World &world, int x, int y)
{
	++y;
	if (!isBlockWalkThrough(world.blocks[x][y + 1]) && isBlockWalkThrough(world.blocks[x][y]) && world.blocks[x][y] != CACTUS && world.blocks[x][y + 1] != CACTUS) return true;
	return false;
}

bool BaseMob::isInBlock(World &world)
{
	int addY = sy / (smallMob ? 1 : 2) - 1;
	for (int x = this->x - sx / 2 + 1; x < this->x + sx / 2 + 1; x += sx - 1)
		for (int y = this->y - sy / 2 + 1; y < this->y + sy / 2; y += addY)
			if (!isBlockWalkThrough(world.blocks[x / 16][y / 16]))
				return true;
	return false;
}

void BaseMob::calcHealth()
{
	if (health>getMaxHealth())
		health=getMaxHealth();
	else if (getGlobalSettings()->getProperty(PROPERTY_REGEN) && health<getMaxHealth() && getTime()%256==0)
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

bool BaseMob::canJump(World *world)
{
	return !collisions[SIDE_TOP] && (collisions[SIDE_BOTTOM] || (world && (isWaterAt(*world, x, y + sy / 2 - 1) || isWaterAt(*world, x, y - sy / 2 + 1)) && vy < 3));
}
