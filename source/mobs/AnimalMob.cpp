#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "AnimalMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../general.h"
#include "../sounds.h"
#include "../collision.h"
#include "../inventory.h"
#include "../mainGame.h"
Graphic animalMobGraphics[3][2];

AnimalMob::AnimalMob()
{
	mov = 0;
	dir = true;
	scaredTimer = 0;
	x = 0;
	y = 0;
	vy = 0;
	vx = 0;
	ping = 0;
	alive = false;
	health = 10;
	framesHurtSprite = 0;
	notarget = 0;
	smallmob = true;
}

AnimalMob::AnimalMob(int a, int b)
{
	mov = 0;
	dir = true;
	scaredTimer = 0;
	sx = 10;
	sy = 16;
	x = a;
	y = b;
	vy = 0;
	vx = 0;
	alive = false;
	facing = false;
	type = MOB_ANIMAL;
	health = 10;
	ping = 0;
	spriteState = 0;
	notarget = 0;
	timeTillWifiUpdate = rand() % 4 + 4;
	smallmob = true;
	animal = AnimalType(rand() % 3);
}

void AnimalMob::updateMob(WorldObject* world)
{
	if (spriteState == 0) showGraphic(&animalMobGraphics[animal][0], x - world->camX - 7, y - world->camY - 7, facing ? true : false);
	else if (spriteState == 1) showGraphic(&animalMobGraphics[animal][1], x - world->camX - 7, y - world->camY - 7, facing ? true : false);

	if (host == true)
	{
		BaseMob_ptr target = mobHandlerFindMob(256, MOB_PLAYER, x, y);

		if (scaredTimer == 1)
		{
			vx = 0;
			scaredTimer = 0;
		}

		if (target->type != MOB_PLAYER) //Out of range of player
		{
			scaredTimer = 0;
			++notarget;
		}
		else if (scaredTimer == 0)
		{
			if (mov > 1)
			{
				if ((collisions[SIDE_RIGHT] || collisions[SIDE_LEFT]) && collisions[SIDE_BOTTOM] && !collisions[SIDE_TOP])
					vy = JUMP_VELOCITY;
				else if (!(collisions[SIDE_RIGHT] || collisions[SIDE_LEFT]))
					--mov;
				if (!collisions[SIDE_BOTTOM] && ((collisions[SIDE_RIGHT] && dir) || (collisions[SIDE_LEFT]&&!dir)) && vy > 0)
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
			vx = (facing = target->x > x) ? -RUN_VELOCITY : RUN_VELOCITY;
			if ((collisions[SIDE_RIGHT] || collisions[SIDE_LEFT]) && collisions[SIDE_BOTTOM] && !collisions[SIDE_TOP])
				vy = JUMP_VELOCITY;
			--scaredTimer;
		}
		if (type == MOB_PLAYER) notarget = 0;
		ping = 0;
		if (framesHurtSprite == 0) spriteState = 0;
		else --framesHurtSprite;
	}
}

void AnimalMob::sendWifiUpdate()
{
}

void AnimalMob::saveToFile(FILE* pFile)
{
}

void AnimalMob::loadFromFile(FILE* pFile)
{
	kill();
}

void AnimalMob::hurt(int amount, int type)
{

	if (spriteState == 1)
		return;
	if (jumpHurtType(type) && collisions[SIDE_BOTTOM])
		vy = JUMP_VELOCITY;
	health -= amount;
	int playerX = getPlayerX();
	if (abs(x - playerX) < 256)
	{
		int volume = 280 - abs(x - playerX);
		if (volume > 255)
			volume = 255;
		int panning = (x - playerX) / 2 + 127;
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

bool canAnimalMobSpawnHere(WorldObject* world, int x, int y)
{
	return canMobSpawnHere(world, x, y);
}

void animalMobInit()
{
	loadGraphic(&animalMobGraphics[ANIMAL_PIG][0], GRAPHIC_MOB, 10, 16, 16);
	loadGraphic(&animalMobGraphics[ANIMAL_PIG][1], GRAPHIC_MOB, 11, 16, 16);
	loadGraphic(&animalMobGraphics[ANIMAL_COW][0], GRAPHIC_MOB, 12, 16, 16);
	loadGraphic(&animalMobGraphics[ANIMAL_COW][1], GRAPHIC_MOB, 13, 16, 16);
	loadGraphic(&animalMobGraphics[ANIMAL_SHEEP][0], GRAPHIC_MOB, 14, 16, 16);
	loadGraphic(&animalMobGraphics[ANIMAL_SHEEP][1], GRAPHIC_MOB, 15, 16, 16);
}