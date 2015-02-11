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
Graphic zombieMobGraphic[3];

zombieMob::zombieMob()
{
	x = 0;
	y = 0;
	vy = 0;
	vx = 0;
	ping = 0;
	alive = false;
	onground = false;
	health = 10;
	mobtype = 0;
	animationclearframes = 0;
	notarget = 0;
}

zombieMob::zombieMob(int a, int b)
{
	jump = 0;
	gravity = 3;
	gravityValue = 3;
	sx = 6;
	sy = 32;
	x = a;
	y = b;
	vy = 0;
	vx = 0;
	alive = false;
	onground = false;
	facing = false;
	mobtype = 3;
	health = 20;
	ping = 0;
	animation = 0;
	notarget = 0;
	timeTillWifiUpdate = rand() % 4 + 4;
}

void zombieMob::hurt(int amount, int type)
{

	if (animation == 1)
		return;
	if (type != VOID_HURT)
		vy -= 2;
	y += vy;
	playSound(SOUND_ZOMBIE_HURT);
	health -= amount;
	animation = 1;
	animationclearframes = 20;
	if (health <= 0)
	{
		if (type == PLAYER_HURT)
			addInventory(FLESH, rand() % 2 + 1);
		killMob();
	}
}

void zombieMob::updateMob(worldObject* world)
{
	if (world->timeInWorld < 80 && rand() % 200 == 1) hurt(2, SUN_HURT);
	if (animation == 0) showGraphic(&zombieMobGraphic[0], x - world->CamX - (facing ? 10 : 0), y - world->CamY, facing ? true : false);
	else if (animation == 1) showGraphic(&zombieMobGraphic[1], x - world->CamX - (facing ? 10 : 0), y - world->CamY, facing ? true : false);
	if (host == true)
	{
		target = mobHandlerFindMob(128, 2, x, y);
		if (target->x < x && target->mobtype == 2) facing = true;
		else if (target->mobtype == 2) facing = false;
		++jump;
		if (collisions[0] == false) y += vy;
		else vy = 0;
		if (target->x > x - 4 && target->x < x + 4)
			jump = 0;
		if (target->mobtype != 2)
		{
			++notarget;
			jump = 0;
		}
		else if (!collisions[1] && facing == false && !collisions[3] && jump > 1)
		{
			x += facing ? -1 : 1;
			jump = 0;
		}
		else if (!collisions[2] && facing == true && !collisions[3] && jump > 1)
		{
			x += facing ? -1 : 1;
			jump = 0;
		}
		else if ((collisions[1] || collisions[2]) && collisions[0] && !collisions[3] && animation != 1)
		{
			vy = -2;
			y += vy;
		}
		if (target->mobtype == 2) notarget = 0;
		if (collisions[3])
		{
			vy = 0;
			y += 3;
		}
		ping = 0;
		if (notarget > 1800) killMob();
		if (animationclearframes == 0) animation = 0;
		else --animationclearframes;
		if (spritecol(x, y, target->x, target->y, sx, sy, target->sx, target->sy))
			mobHandlerHurtMob(target->mobId, 1, ZOMBIE_HURT);
		target = mobHandlerFindMob(512, 2, x, y);
		if (target->mobtype == 2) notarget = 0;
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

bool canZombieMobSpawnHere(worldObject* world, int x, int y)
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
