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
Graphic zombieMobGraphic[3];

void ZombieMob::calcMiscData(WorldObject* world)
{
	calculateMiscData(world, this);
}

void ZombieMob::hurt(int amount, int type)
{

	if (spriteState == 1)
		return;
	if (jumpHurtType(type) && collisions[SIDE_BOTTOM])
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

void ZombieMob::updateMob(WorldObject* world)
{
	if (world->timeInWorld < 80 && rand() % 200 == 1) hurt(2, SUN_HURT);
	if (spriteState == 0) showGraphic(&zombieMobGraphic[0], x - world->camX - 7, y - world->camY - 15, facing ? true : false);
	else if (spriteState == 1) showGraphic(&zombieMobGraphic[1], x - world->camX - 7, y - world->camY - 15, facing ? true : false);
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
		if ((collisions[SIDE_RIGHT] || collisions[SIDE_LEFT]) && collisions[SIDE_BOTTOM] && !collisions[SIDE_TOP] && spriteState != 1)
			vy = JUMP_VELOCITY;
		if (framesHurtSprite == 0) spriteState = 0;
		else --framesHurtSprite;
		if (spriteCol(x, y, target->x, target->y, sx, sy, target->sx, target->sy))
			target->hurt(1, ZOMBIE_HURT);
	}
}

void ZombieMob::sendWifiUpdate()
{
}

void ZombieMob::saveToFile(FILE* pFile)
{
}

void ZombieMob::loadFromFile(FILE* pFile)
{
	health = 0;
}

bool canZombieMobSpawnHere(WorldObject* world, int x, int y)
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
	loadGraphic(&zombieMobGraphic[0], GRAPHIC_MOB, 3);
	loadGraphic(&zombieMobGraphic[1], GRAPHIC_MOB, 4);
}

bool ZombieMob::isMyPlayer()
{
	return false;
}
