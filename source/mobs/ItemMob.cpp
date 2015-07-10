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

bool ItemMob::isMyPlayer()
{
	return false;
}

void ItemMob::calcMiscData(WorldObject* world)
{
	
}

void ItemMob::updateMob(WorldObject* world)
{
	if (world->gamemode == GAMEMODE_CREATIVE)
	{
		health = 0;
		return;
	}
	if (!itemGraphic)
	{
		itemGraphic = new Graphic();
		itemGraphic->paletteID = 3 + (12 * (brightness = getBrightness(world, x / 16, (y - 8) / 16 + 1))) / 15;
		loadGraphic(itemGraphic, GRAPHIC_BLOCK_MINI, displayID, 8, 8, 3 + (12 * brightness) / 15);
	}
	if (health<1)
		return;
	if (vx != 0)
	{
		if ((vx > 0 && isBlockWalkThrough(world->blocks[int ((x + sx / 2 + 1) / 16)][int(y) / 16])) || (vx < 0 && isBlockWalkThrough(world->blocks[int(x - sx / 2) / 16][int(y) / 16])))
		{
			bool positive = vx > 0;
			vx -= positive ? 1.0 / 120.0 : -1.0 / 120.0;
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
	if (isBlockWalkThrough(world->blocks[int(x - sx / 2 + 1) / 16][int(y + sy / 2) / 16]) && isBlockWalkThrough(world->blocks[int(x + sx / 2) / 16][int(y + sy / 2) / 16]))
	{
		y += 16.0 * vy / double(FPS);
		vy += (18.0 / 60.0);
	}
	if (vy != 0 && !isBlockWalkThrough(world->blocks[int(x) / 16][int(y + sy / 2 - 1) / 16]))
	{
		y -= int(y + sy / 2) % 16;
		vy = 0;
	}
	if (world->blocks[int(x) / 16][(int(y) - 8) / 16 + 1] != AIR && getBrightness(world, x / 16, (y - 8) / 16 + 1) != brightness)
		itemGraphic->paletteID = 3 + (12 * (brightness = getBrightness(world, x / 16, (y - 8) / 16 + 1))) / 15;
	++floatY;
	if (floatY > 100)
		floatY = 0;
	BaseMob_ptr target = mobHandlerFindMob(8, MOB_PLAYER, x, y - 8);
	if (target == NULL)
		target = mobHandlerFindMob(8, MOB_PLAYER, x, y - 24);
	if (target == NULL || !target->isMyPlayer())
		showGraphic(itemGraphic, x - world->camX - 3, (y - 8 - world->camY + int((4.0 * sin(double(floatY)*6.28 / 100.0)))), false);
	else if (addInventory(blockID, amount))
		health = 0;
	if (!onScreen(x, y, world->camX, world->camY) && rand() % 1000 == 1)
		health = 0;
	if (health<1)
		playSound(SOUND_POP, 155 + rand() % 100);
}

void ItemMob::sendWifiUpdate()
{
}

void ItemMob::saveToFile(FILE* pFile)
{
}

void ItemMob::loadFromFile(FILE* pFile)
{
	health = 0;
}

void ItemMob::hurt(int hamount, int type)
{
	switch (type)
	{
		case CACTUS_HURT:
			if (rand() % 10 == 1)
				health = 0;;
			break;
		default:
			break;
	}
}
