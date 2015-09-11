#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "MultiplayerMob.h"
#include "../blockID.h"
#include "../worldRender.h"
#include <nds.h>
//ASDF?

void MultiplayerMob::calcMiscData(WorldObject &world) { }

void MultiplayerMob::hurt(int amount, int type)
{
	return; //Do nothing, if a Multiplayer mob is hurt then it should be realised on the server not client...
}

void MultiplayerMob::updateMob(WorldObject &world)
{
	if (brightness<0)
	{
		loadGraphic(&normalSprite, GRAPHIC_MOB, 0, 16, 32, 8+(6 * (brightness = getBrightness(world, x / 16, (y+8) / 16 + 1))) / 15);
		loadGraphic(&hurtSprite, GRAPHIC_MOB, 1, 16, 32, normalSprite.paletteID);
	}
	if (spriteState == 0) showGraphic(&normalSprite, x - world.camX - (facing ? 10 : 0), y - world.camY, facing ? true : false);
	else if (spriteState == 1) showGraphic(&hurtSprite, x - world.camX - (facing ? 10 : 0), y - world.camY, facing ? true : false);
	if (world.blocks[int(x) / 16][(int(y+8)) / 16 + 1] != AIR && getBrightness(world, x / 16, (y+8) / 16 + 1) != brightness)
		hurtSprite.paletteID = normalSprite.paletteID = 8+(6 * (brightness = getBrightness(world, x / 16, (y+8) / 16 + 1))) / 15;
}

void MultiplayerMob::sendWifiUpdate() { }

void MultiplayerMob::saveToFile(FILE* pFile) { }

void MultiplayerMob::loadFromFile(FILE* pFile)
{
	health = 0;
}

bool canMultiplayerMobSpawnHere(WorldObject &world, int x, int y)
{
	++y;
	if (!isBlockWalkThrough(world.blocks[x][y + 1]) && isBlockWalkThrough(world.blocks[x][y]) && world.blocks[x][y] != CACTUS)
		return true;
	return false;
}