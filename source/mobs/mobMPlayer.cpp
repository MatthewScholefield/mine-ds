#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "mobMPlayer.h"
#include "../blockID.h"
#include <nds.h>
//ASDF?
Graphic MplayerMobGraphic[3];

MplayerMob::MplayerMob()
{
	x = 0;
	y = 0;
	vy = 0;
	vx = 0;
	alive = false;
	animation = 0;
	sx = 6;
	sy = 32;
	mobType = 2;
	ping = 0;
}

MplayerMob::MplayerMob(int a, int b)
{
	sx = 6;
	sy = 32;
	x = a;
	y = b;
	vy = 0;
	vx = 0;
	alive = false;
	animation = 0;
	mobType = 2;
	ping = 0;
}

void MplayerMob::hurt(int amount, int type)
{
	return; //Do nothing, if a Multiplayer mob is hurt then it should be realised on the server not client...
}

void MplayerMob::updateMob(WorldObject* world)
{
	if (x - world->camX>-16 && x - world->camX < 256 + 16 && y - world->camY>-32 && y - world->camY < 256)
	{
		if (animation == 0) showGraphic(&MplayerMobGraphic[0], x - world->camX - (facing ? 10 : 0), y - world->camY, facing ? true : false);
		else if (animation == 1) showGraphic(&MplayerMobGraphic[1], x - world->camX - (facing ? 10 : 0), y - world->camY, facing ? true : false);
	}
}

void MplayerMob::sendWifiUpdate()
{

}

void MplayerMob::saveToFile(FILE* pFile)
{
}

void MplayerMob::loadFromFile(FILE* pFile)
{
	killMob();
}

bool canMplayerMobSpawnHere(WorldObject* world, int x, int y)
{
	++y;
	if (!isBlockWalkThrough(world->blocks[x][y + 1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y] != CACTUS)
		return true;
	return false;
}

void MplayerMobInit()
{
	loadGraphic(&MplayerMobGraphic[0], true, 0);
	loadGraphic(&MplayerMobGraphic[1], true, 1);
}

