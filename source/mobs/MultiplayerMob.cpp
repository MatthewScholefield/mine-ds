#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "MultiplayeMob.h"
#include "../blockID.h"
#include <nds.h>
//ASDF?
Graphic MplayerMobGraphic[3];

void MultiplayerMob::calcMiscData(WorldObject* world)
{
	
}

void MultiplayerMob::hurt(int amount, int type)
{
	return; //Do nothing, if a Multiplayer mob is hurt then it should be realised on the server not client...
}

void MultiplayerMob::updateMob(WorldObject* world)
{
	if (x - world->camX>-16 && x - world->camX < 256 + 16 && y - world->camY>-32 && y - world->camY < 256)
	{
		if (spriteState == 0) showGraphic(&MplayerMobGraphic[0], x - world->camX - (facing ? 10 : 0), y - world->camY, facing ? true : false);
		else if (spriteState == 1) showGraphic(&MplayerMobGraphic[1], x - world->camX - (facing ? 10 : 0), y - world->camY, facing ? true : false);
	}
}

void MultiplayerMob::sendWifiUpdate()
{

}

void MultiplayerMob::saveToFile(FILE* pFile)
{
}

void MultiplayerMob::loadFromFile(FILE* pFile)
{
	health = 0;
}

bool canMultiplayerMobSpawnHere(WorldObject* world, int x, int y)
{
	++y;
	if (!isBlockWalkThrough(world->blocks[x][y + 1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y] != CACTUS)
		return true;
	return false;
}

void multiplayerMobInit()
{
	loadGraphic(&MplayerMobGraphic[0], GRAPHIC_MOB, 0);
	loadGraphic(&MplayerMobGraphic[1], GRAPHIC_MOB, 1);
}

