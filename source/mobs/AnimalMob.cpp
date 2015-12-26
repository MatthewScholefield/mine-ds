#include <stdio.h>
#include "../world/WorldObject.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "AnimalMob.h"
#include "mobHandler.h"
#include "mobFunctions.h"
#include "../blockID.h"
#include "../general.h"
#include "../sounds.h"
#include "../collision.h"
#include "../inventory.h"
#include "../mainGame.h"
#include "../world/worldRender.h"
const int AnimalMob::FRAME[3] = {10, 12, 14};

void AnimalMob::calcMiscData(WorldObject &world)
{
	calculateMiscDataSmall(world, this);
}

void AnimalMob::updateMob(WorldObject &world)
{
	if (brightness<0)
		calcMobBrightness(world);
	
	showGraphic(spriteState == 0 ? &normalSprite : &hurtSprite, x - world.camX - 7, y - world.camY - 7, facing ? true : false);

	if (host == true)
	{
		BaseMob_ptr target = mobHandlerFindMob(256, MOB_PLAYER, x, y);

		if (scaredTimer == 1)
		{
			vx = 0;
			scaredTimer = 0;
		}

		if (target->type != MOB_PLAYER) //Out of range of player
			scaredTimer = 0;
		else if (scaredTimer == 0)
		{
			if (mov > 1)
			{
				if ((collisions[SIDE_RIGHT] || collisions[SIDE_LEFT]) && canJump(&world))
					vy = JUMP_VELOCITY;
				else if (!(collisions[SIDE_RIGHT] || collisions[SIDE_LEFT]))
					--mov;
				if (!canJump(&world) && ((collisions[SIDE_RIGHT] && dir) || (collisions[SIDE_LEFT]&&!dir)) && vy > 0)
				{
					mov = 50;
					dir = !(facing = dir); //Change Direction
					vx = (facing) ? -WALK_VELOCITY : WALK_VELOCITY;
				}
			}
			else if (mov == 1)
			{
				vx = 0;
				mov = 0;
			}
			else if (rand() % 50 == 0) //Begin to move
			{
				mov = 60 + (rand() % 40);
				if (rand() % 2 == 0)
				{
					dir = !(facing = false);
					vx = WALK_VELOCITY;
				}
				else
				{
					dir = !(facing = true);
					vx = -WALK_VELOCITY;
				}
			}
		}
		else //Scared
		{
			vx = (facing = (target->x > x)) ? -RUN_VELOCITY : RUN_VELOCITY;
			if ((collisions[SIDE_RIGHT] || collisions[SIDE_LEFT]) && canJump(&world))
				vy = JUMP_VELOCITY;
			--scaredTimer;
		}
		if (framesHurtSprite == 0) spriteState = 0;
		else --framesHurtSprite;
	}
	if (world.blocks[int(x) / 16][(int(y)) / 16 + 1] != AIR && world.brightness[x / 16][y / 16 + 1] != brightness)
		calcMobBrightness(world);
}

void AnimalMob::sendWifiUpdate() { }

void AnimalMob::saveToFile(FILE* pFile) { }

void AnimalMob::loadFromFile(FILE* pFile)
{
	health = 0;
}

void AnimalMob::hurt(int amount, int type)
{

	if (spriteState == 1)
		return;
	if (jumpHurtType(type) && canJump())
		vy = JUMP_VELOCITY;
	health -= amount;
	int playerX = getPlayerX();
	if (abs(x - playerX) < 256)
	{
		int volume = - abs(x - playerX) + 260;
		if (volume > 255)
			volume = 255;
		if (volume < 0)
			volume = 0;
		int panning = 127 + x - playerX;
		if (panning < 0)
			panning = 0;
		if (panning > 255)
			panning = 255;
		switch (animal)
		{
		case ANIMAL_COW:
			playSound(SOUND_COW_HURT, volume, panning);
			break;
		case ANIMAL_SHEEP:
			playSound(SOUND_SHEEP_HURT, volume, panning);
			break;
		case ANIMAL_PIG:
			playSound(SOUND_PIG_HURT, volume, panning);
			break;
		}
	}
	if (type == PLAYER_HURT)
		scaredTimer = 200;
	if (health <= 0)
	{
		switch (animal)
		{
		case ANIMAL_COW:
			createItemMob(x / 16, y / 16, BEEF_RAW, rand() % 4);
			createItemMob(x / 16, y / 16, LEATHER, rand() % 3);
			break;
		case ANIMAL_SHEEP:
			createItemMob(x / 16, y / 16, WHITE_WOOL, rand() % 4);
			break;
		case ANIMAL_PIG:
			createItemMob(x / 16, y / 16, PORKCHOP_RAW, rand() % 3);
			break;
		}
	}
	spriteState = 1;
	framesHurtSprite = 20;
}

bool AnimalMob::isMyPlayer()
{
	return false;
}

bool canAnimalMobSpawnHere(WorldObject &world, int x, int y)
{
	return canMobSpawnHere(world, x, y);
}