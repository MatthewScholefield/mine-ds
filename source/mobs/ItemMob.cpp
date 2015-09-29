#include <stdio.h>
#include <math.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "ItemMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../blocks.h"
#include "../general.h"
#include "../sounds.h"
#include "../collision.h"
#include "../inventory.h"
#include "mobFunctions.h"
#include "../mining.h"
#include "../worldRender.h"

const int ItemMob::floatVal[] = {0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -3, -3, -3, -3, -3, -3, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0};

bool ItemMob::isMyPlayer()
{
	return false;
}

void ItemMob::calcMiscData(WorldObject &world) { }

void ItemMob::updateMob(WorldObject &world)
{
	if (world.gameMode == GAMEMODE_CREATIVE)
	{
		health = 0;
		return;
	}
	if (brightness < 0)
	{
		bool hasLoaded = loadGraphic(&normalSprite, GRAPHIC_BLOCK_MINI, displayID, 8, 8, (7 * (brightness = getBrightness(world, x / 16, (y - 8) / 16 + 1))) / 15);
		if (!hasLoaded)
		{
			health = 0;
			return;
		}
	}
	if (health < 1)
		return;
	if (vx < 0 || vx > 0)
	{
		if ((vx > 0 && isBlockWalkThrough(world.blocks[int ((x + sx / 2 + 1) / 16)][int(y) / 16])) || (vx < 0 && isBlockWalkThrough(world.blocks[int(x - sx / 2) / 16][int(y) / 16])))
		{
			bool positive = vx > 0;
			vx -= FixedPoint(true, (positive ? 0.01 : -0.01) * FixedPoint::SCALER);
			if ((positive && vx < 0) || (!positive && vx > 0))
				vx = 0;
			x += vx;
		}
		else
		{
			x -= vx;
			vx = 0;
		}
	}
	if (isBlockWalkThrough(world.blocks[int(x - sx / 2 + 1) / 16][int(y + sy / 2) / 16]) && isBlockWalkThrough(world.blocks[int(x + sx / 2) / 16][int(y + sy / 2) / 16]))
	{
		y += (16 * vy) / FPS;
		vy += FixedPoint(true, (18 * FixedPoint::SCALER) / FPS); //Gravity Acceleration = +18.0 m/s^2
	}
	bool falling = vy != 0 && !isBlockWalkThrough(world.blocks[int(x) / 16][int(y + sy / 2 - 1) / 16]);
	if (falling)
	{
		y -= int(y + sy / 2) % 16;
		vy = 0;
	}
	else if (++floatY > 100)
		floatY = 0;
	if (world.blocks[int(x) / 16][(int(y) - 8) / 16 + 1] != AIR && getBrightness(world, x / 16, (y - 8) / 16 + 1) != brightness)
		normalSprite.paletteID = (7 * (brightness = getBrightness(world, x / 16, (y - 8) / 16 + 1))) / 15;
	BaseMob_ptr target = mobHandlerFindMob(8, MOB_PLAYER, x, y - 8);
	if (target == nullptr)
		target = mobHandlerFindMob(8, MOB_PLAYER, x, y - 24);
	if (target == nullptr || !target->isMyPlayer())
		showGraphic(&normalSprite, (int) x - world.camX - 3, (int) y - 9 - world.camY + floatVal[floatY], false);
	else if (addInventory(blockID, amount))
		health = 0;
	if (!onScreen(x, y, world.camX, world.camY) && rand() % 1000 == 1)
		health = 0;
	if (health < 1)
		playSound(SOUND_POP, 155 + rand() % 100);
}

void ItemMob::sendWifiUpdate() { }

void ItemMob::saveToFile(FILE* pFile)
{
	int i = x;
	int j = y;
	fprintf(pFile,"%d %d %d %d %d ",i, j, blockID, amount, displayID);
}

void ItemMob::loadFromFile(FILE* pFile)
{
	int i,j;
	fscanf(pFile,"%d %d %d %d %d ",&i, &j, &blockID, &amount, &displayID);
	x = i;
	y = j;
	health = 100;
}

void ItemMob::hurt(int hamount, int type)
{
	switch (type)
	{
	case CACTUS_HURT:
		if (rand() % 10 == 1)
			health = 0;
		break;
	default:
		break;
	}
}
