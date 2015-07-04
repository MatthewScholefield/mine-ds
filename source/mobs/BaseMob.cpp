#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "BaseMob.h"
#include "../blockID.h"
#include "hurt.h"
//ASDF?
Graphic baseMobGraphic[3];

BaseMob::BaseMob()
{
	host = true;
	facing = false;
	for (int i = 0; i < 5; ++i)
		collisions[i] = false;
	mobId = 0;
	timeTillWifiUpdate = rand() % 4 + 4;
	animation = 0;
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
	mobType = 0;
	animationClearFrames = 0;

}

BaseMob::BaseMob(int a, int b)
{
	host = true;
	for (int i = 0; i < 5; ++i)
		collisions[i] = false;
	mobId = 0;
	animation = 0;
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
	mobType = 0;
	health = 10;
	animationClearFrames = 0;
	timeTillWifiUpdate = rand() % 4 + 4;
}

/*void BaseMob::resetVelocity()
{
	vy = 0;
	vx = 0;
}*/

void BaseMob::updateMob(WorldObject* world)
{
	if (animation == 0) showGraphic(&baseMobGraphic[0], x - world->camX, y - world->camY);
	else if (animation == 1) showGraphic(&baseMobGraphic[1], x - world->camX, y - world->camY);
	if (host == true)
	{
		ping = 0;
		if (health <= 0)
			killMob();
		if (animationClearFrames == 0) animation = 0;
		else --animationClearFrames;
	}
}

/*void BaseMob::setXYPos(int a, int b)
{
	x = a;
	y = b;
}*/

void BaseMob::sendWifiUpdate()
{
}

void BaseMob::saveToFile(FILE* pFile)
{
}

void BaseMob::loadFromFile(FILE* pFile)
{
	killMob();
}

bool canBaseMobSpawnHere(WorldObject* world, int x, int y)
{
	++y;
	if (!isBlockWalkThrough(world->blocks[x][y + 1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y] != CACTUS && world->blocks[x][y + 1] != CACTUS) return true;
	return false;
}

void BaseMobInit()
{
	loadGraphic(&baseMobGraphic[0], GRAPHIC_MOB, 0);
	loadGraphic(&baseMobGraphic[1], GRAPHIC_MOB, 1);
}

void BaseMob::hurt(int amount, int type)
{
	health -= amount;
	animation = 1;
	animationClearFrames = 20;
}

void BaseMob::killMob()
{
	timeTillWifiUpdate = 1;
	alive = false;
}

void BaseMob::unKillMob()
{
	alive = true;
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