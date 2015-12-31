#include <stdio.h>
#include "../world/World.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "MultiplayerMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../world/worldRender.h"
#include <nds.h>
//ASDF?

void MultiplayerMob::calcMiscData(World &world) { }

void MultiplayerMob::hurt(int amount, int type)
{
	return; //Do nothing, if a Multiplayer mob is hurt then it should be realised on the server not client...
}

void MultiplayerMob::updateMob(World &world)
{
	if (brightness<0)
		calcMobBrightness(world);
	getSprite().draw(x - world.camX - (facing ? 10 : 0), y - world.camY, facing ? true : false);
	if (world.brightness[x / 16][(y+8) / 16 + 1] != brightness)
		calcMobBrightness(world);
}

void MultiplayerMob::sendWifiUpdate() { }

bool canMultiplayerMobSpawnHere(World &world, int x, int y)
{
	return canMobSpawnHere(world, x, y);
}