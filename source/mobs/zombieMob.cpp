#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "zombieMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../general.h"
#include "../collision.h"
#include "../worldRender.h"
#include "../sounds.h"
#include "../inventory.h"
#include "../mainGame.h"
Graphic zombieMobGraphic[3];

zombieMob::zombieMob()
{
	target = NULL;
	jump = 0;
	x = 0;
	y = 0;
	vy = 0;
	vx = 0;
	ping = 0;
	alive = false;
	health = 10;
	mobType = 0;
	animationClearFrames = 0;
	noTarget = 0;
}

zombieMob::zombieMob(int a, int b)
{
	target = NULL;
	jump = 0;
	sx = 6;
	sy = 32;
	x = a;
	y = b;
	vy = 0;
	vx = 0;
	alive = false;
	facing = false;
	mobType = 3;
	health = 20;
	ping = 0;
	animation = 0;
	noTarget = 0;
	timeTillWifiUpdate = rand() % 4 + 4;
}

void zombieMob::hurt(int amount, int type)
{

	if (animation == 1)
		return;
	if (type != VOID_HURT && collisions[0])
		vy = JUMP_VELOCITY;
	playSound(SOUND_ZOMBIE_HURT);
	health -= amount;
	animation = 1;
	animationClearFrames = 20;
	if (health <= 0)
	{
		createItemMob(x, y, FLESH, rand() % 2 + 1);
		killMob();
	}
}

void zombieMob::updateMob(WorldObject* world)
{
	if (world->timeInWorld < 80 && rand() % 200 == 1) hurt(2, SUN_HURT);
	if (animation == 0) showGraphic(&zombieMobGraphic[0], x - world->camX - 7, y - world->camY - 15, facing ? true : false);
	else if (animation == 1) showGraphic(&zombieMobGraphic[1], x - world->camX - 7, y - world->camY - 15, facing ? true : false);
	if (host == true)
	{
		target = mobHandlerFindMob(128, 2, x, y);
		if (target->x < x && target->mobType == 2) facing = true;
		else if (target->mobType == 2) facing = false;
		++jump;
		if (target->x > x - 4 && target->x < x + 4)
			jump = 0;
		if (target->mobType != 2)
		{
			++noTarget;
			jump = 0;
		}
		else if (!collisions[1] && facing == false && !collisions[3] && jump > 1)
		{
			vx = facing ? -1 : 1;
			jump = 0;
		}
		else if (!collisions[2] && facing == true && !collisions[3] && jump > 1)
		{
			vx = facing ? -1 : 1;
			jump = 0;
		}
		else
			vx = 0;
		if ((collisions[1] || collisions[2]) && collisions[0] && !collisions[3] && animation != 1)
			vy = JUMP_VELOCITY;
		if (target->mobType == 2) noTarget = 0;
		ping = 0;
		if (noTarget > 1800) killMob();
		if (animationClearFrames == 0) animation = 0;
		else --animationClearFrames;
		if (spriteCol(x, y, target->x, target->y, sx, sy, target->sx, target->sy))
			mobHandlerHurtMob(target->mobId, 1, ZOMBIE_HURT);
		target = mobHandlerFindMob(512, 2, x, y);
		if (target->mobType == 2) noTarget = 0;
	}
}

void zombieMob::sendWifiUpdate()
{
}

void zombieMob::saveToFile(FILE* pFile)
{
}

void zombieMob::loadFromFile(FILE* pFile)
{
	killMob();
}

bool canZombieMobSpawnHere(WorldObject* world, int x, int y)
{
	++y;
	if (!isBlockWalkThrough(world->blocks[x][y + 1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y] != CACTUS && world->blocks[x][y + 1] != CACTUS)
	{
		if (getBrightness(world, x, y + 1) > 7)
			return true;
	}
	return false;
}

void zombieMobInit()
{
	loadGraphic(&zombieMobGraphic[0], true, 3);
	loadGraphic(&zombieMobGraphic[1], true, 4);
}

bool zombieMob::isMyPlayer()
{
	return false;
}
