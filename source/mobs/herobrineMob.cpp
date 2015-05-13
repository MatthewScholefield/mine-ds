#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "herobrineMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../collision.h"
#include "../worldRender.h"
#include "../inventory.h"
#include "../mining.h"
#include "../general.h"
//#include "../sounds.h"

Graphic herobrineMobGraphic[3];

herobrineMob::herobrineMob()
{
	target = NULL;
	jump = 0;
	x = 0;
	y = 0;
	vy = 0;
	vx = 0;
	ping = 0;
	alive = false;
	health = 7;
	mobType = 0;
	animationClearFrames = 0;
	notarget = 0;
	waitingCount = -rand() % 4000;
}

herobrineMob::herobrineMob(int a, int b)
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
	health = 7;
	ping = 0;
	animation = 0;
	notarget = 0;
	timeTillWifiUpdate = rand() % 4 + 4;
	waitingCount = -rand() % 4000;
}

void herobrineMob::updateMob(WorldObject* world)
{
	++waitingCount;
	if (animation == 0) showGraphic(&herobrineMobGraphic[0], x - world->camX - 7, y - world->camY - 15, facing ? true : false);
	else if (animation == 1) showGraphic(&herobrineMobGraphic[1], x - world->camX - 7, y - world->camY - 15, facing ? true : false);
	if (host == true)
	{
		if (collisions[0] && collisions[3])
			while (y > 16 && (world->blocks[int(x) / 16][(int(y) / 16) + 1] != AIR || world->blocks[int(x) / 16][int(y) / 16] != AIR))
				y -= 16;
		target = mobHandlerFindMob(128, 2, x, y);
		if (target->x < x && target->mobType == 2) facing = true;
		else if (target->mobType == 2) facing = false;
		++jump;
		int distance = target->x - x;
		if (distance < 0)
			distance *= -1;
		if (target->mobType != 2)
		{
			++notarget;
			jump = 0;
		}
		else if (!collisions[1] && !collisions[3] && jump > 1)
		{
			if (waitingCount > 1000)
			{
				waitingCount = 1100;
				vx = ((facing ? -1 : 1) * rand() % (distance < 40 && rand() % 2 ? 2 : 3));
			}
			else
			{
				if (!(distance < 65 && distance > 55))
					x += ((facing ? -1 : 1)*(distance > 60 ? 1 : -1) * rand() % 5);
				else if (rand() % 10 == 1)
					x += ((rand() % 2) ? -1 : 1)*(rand() % 15);
				jump = 0;
			}
		}
		else if ((collisions[1] || collisions[2]) && collisions[0] && !collisions[3] && animation != 1)
			vy = JUMP_VELOCITY;
		if (target->mobType == 2) notarget = 0;
		ping = 0;
		if (notarget > 1800) killMob();
		if (animationClearFrames == 0) animation = 0;
		else --animationClearFrames;
		if (spriteCol(x, y, target->x, target->y, sx, sy, target->sx, target->sy) && waitingCount > 1000)
			mobHandlerHurtMob(target->mobId, 3, HEROBRINE_HURT);
		target = mobHandlerFindMob(512, 2, x, y);
		if (target->mobType == 2) notarget = 0;
	}
}

void herobrineMob::sendWifiUpdate()
{
}

void herobrineMob::saveToFile(FILE* pFile)
{
}

void herobrineMob::loadFromFile(FILE* pFile)
{
	killMob();
}

bool canHerobrineMobSpawnHere(WorldObject* world, int x, int y)
{
	++y;
	if (!isBlockWalkThrough(world->blocks[x][y + 1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y] != CACTUS && world->blocks[x][y + 1] != CACTUS)
	{
		/*if (getBrightness(world, x, y + 1) > 7)*/
		return true;
	}
	return false;
}

void herobrineMobInit()
{
	loadGraphic(&herobrineMobGraphic[0], true, 8);
	loadGraphic(&herobrineMobGraphic[1], true, 9);
}

void herobrineMob::hurt(int amount, int type)
{

	if (animation == 1)
		return;
	if (type != VOID_HURT && collisions[0])
		vy = JUMP_VELOCITY;
	//playSound(HEROBRINE_H);
	health -= amount;
	animation = 1;
	animationClearFrames = 20;
	if (health <= 0)
	{
		if (type == CACTUS_HURT)
		{
			addInventory(rand() % (NUM_BLOCKS - 2) + 2, rand() % 4 + 1);
			killMob();
		}
		else
		{
			x = getDefaultSpawnX()*16;
			waitingCount = -rand() % 4000;
			health = 7;
		}
	}
}

bool herobrineMob::isMyPlayer()
{
	return false;
}
