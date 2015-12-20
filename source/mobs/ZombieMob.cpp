#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "ZombieMob.h"
#include "mobHandler.h"
#include "mobFunctions.h"
#include "../blockID.h"
#include "../general.h"
#include "../collision.h"
#include "../worldRender.h"
#include "../sounds.h"
#include "../inventory.h"
#include "../mainGame.h"

void ZombieMob::calcMiscData(WorldObject &world)
{
	calculateMiscData(world, this);
}

void ZombieMob::hurt(int amount, int type)
{

	if (spriteState == 1)
		return;
	if (jumpHurtType(type) && canJump())
		vy = JUMP_VELOCITY;
	int playerX = getPlayerX();
	if (abs(x - playerX) < 256)
	{
		int volume = 280 - abs(x - playerX);
		if (volume > 255)
			volume = 255;
		playSound(SOUND_ZOMBIE_HURT, volume, (x - playerX) / 2 + 127);
	}
	health -= amount;
	spriteState = 1;
	framesHurtSprite = 20;
	if (health <= 0)
	{
		createItemMob(x, y, FLESH, rand() % 2 + 1);
		health = 0;
	}
}

void ZombieMob::updateMob(WorldObject &world)
{
	if (brightness<0)
	{
		bool loadedFirst = loadGraphic(&normalSprite, GRAPHIC_MOB, 3, 16, 32, 8 + (6 * (brightness = getBrightness(world, x / 16, (y + 8) / 16 + 1))) / 15);
		bool loadedSecond = loadGraphic(&hurtSprite, GRAPHIC_MOB, 4, 16, 32, normalSprite.paletteID);
		if (!loadedFirst || !loadedSecond)
		{
			unloadGraphic(&normalSprite);
			health = 0;
			return;
		}

	}
	if (world.timeInWorld < 80 && rand() % 200 == 1) hurt(2, SUN_HURT);
	if (spriteState == 0) showGraphic(&normalSprite, x - world.camX - 7, y - world.camY - 15, facing ? true : false);
	else if (spriteState == 1) showGraphic(&hurtSprite, x - world.camX - 7, y - world.camY - 15, facing ? true : false);
	if (host == true)
	{
		BaseMob_ptr target = mobHandlerFindMob(128, MOB_PLAYER, x, y);
		if (target->x < x && target->type == MOB_PLAYER) facing = true;
		else if (target->type == MOB_PLAYER) facing = false;
		if (!collisions[SIDE_RIGHT] && facing == false && !collisions[SIDE_TOP])
		{
			if (vx <= 0)
				vx = double(rand() % 15) / 10.0 + ZOMBIE_SPEED;
		}
		else if (!collisions[SIDE_LEFT] && facing == true && !collisions[SIDE_TOP])
		{
			if (vx >= 0)
				vx = -double(rand() % 15) / 10.0 - ZOMBIE_SPEED;
		}
		else
			vx = 0;
		if ((collisions[SIDE_RIGHT] || collisions[SIDE_LEFT]) && canJump(&world))
			vy = JUMP_VELOCITY;
		if (framesHurtSprite == 0) spriteState = 0;
		else --framesHurtSprite;
		if (spriteCol(x, y, target->x, target->y, sx, sy, target->sx, target->sy))
			target->hurt(1, ZOMBIE_HURT);
	}
	if (world.blocks[int(x) / 16][(int(y+8)) / 16 + 1] != AIR && getBrightness(world, x / 16, (y+8) / 16 + 1) != brightness)
		hurtSprite.paletteID = normalSprite.paletteID = 8+(6 * (brightness = getBrightness(world, x / 16, (y+8) / 16 + 1))) / 15;
}

void ZombieMob::sendWifiUpdate() { }

void ZombieMob::saveToFile(FILE* pFile) { }

void ZombieMob::loadFromFile(FILE* pFile)
{
	health = 0;
}

bool canZombieMobSpawnHere(WorldObject &world, int x, int y)
{
	++y;
  if (y >= WORLD_HEIGHT) return false;
  if (y < 0) return false;
  if (x < 0) return false;
  if (x >= WORLD_WIDTH) return false;
	if (!isBlockWalkThrough(world.blocks[x][y + 1]) && isBlockWalkThrough(world.blocks[x][y]) && world.blocks[x][y] != CACTUS && world.blocks[x][y + 1] != CACTUS)
	{
		if (getBrightness(world, x, y + 1) <= 8)
			return true;
	}
	return false;
}

bool ZombieMob::isMyPlayer()
{
	return false;
}
