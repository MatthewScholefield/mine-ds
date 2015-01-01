#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "baseMob.h"
#include "../blockID.h"
//ASDF?
Graphic baseMobGraphic[3];

baseMob::baseMob()
{
	timeOnCactus = 30;
	onCactus = false;
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

}

baseMob::baseMob(int a, int b)
{
	onCactus = false;
	timeOnCactus = 30;
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
	ping = 0;
	mobtype = 0;
	health = 10;
	animationclearframes = 0;
	timeTillWifiUpdate = rand() % 4 + 4;
}

void baseMob::resetVelocity()
{
	vy = 0;
	vx = 0;
}

void baseMob::updateMob(worldObject* world)
{
	if (animation == 0) showGraphic(&baseMobGraphic[0], x - world->CamX, y - world->CamY);
	else if (animation == 1) showGraphic(&baseMobGraphic[1], x - world->CamX, y - world->CamY);
	if (host == true)
	{
		ping = 0;
		if (health <= 0)
			killMob();
		if (collisions[0] == false) y += vy;
		else vy = 0;
		if (animationclearframes == 0) animation = 0;
		else --animationclearframes;
	}
}

void baseMob::setXYPos(int a, int b)
{
	x = a;
	y = b;
}

void baseMob::sendWifiUpdate()
{
}

void baseMob::saveToFile(FILE* pFile)
{
}

void baseMob::loadFromFile(FILE* pFile)
{
}

bool canBaseMobSpawnHere(worldObject* world, int x, int y)
{
	++y;
	if (!isBlockWalkThrough(world->blocks[x][y + 1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y] != CACTUS && world->blocks[x][y + 1] != CACTUS) return true;
	return false;
}

void baseMobInit()
{
	loadGraphic(&baseMobGraphic[0], true, 0);
	loadGraphic(&baseMobGraphic[1], true, 1);
}

void baseMob::hurt(int amount, int type)
{

	health -= amount;
	animation = 1;
	animationclearframes = 20;
}

void baseMob::killMob()
{
	timeTillWifiUpdate = 1;
	alive = false;
}

void baseMob::unKillMob()
{
	alive = true;
}

bool baseMob::isMyPlayer()
{
	return false;
}
