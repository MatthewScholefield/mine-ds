#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "animalMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../general.h"
#include "../sounds.h"
#include "../collision.h"
#include "../inventory.h"
#include "../mainGame.h"
Graphic animalGraphics[3][2];

animalMob::animalMob()
{
	target = NULL;
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
	mobType = 0;
	animationClearFrames = 0;
	notarget = 0;
	smallmob = true;
}

animalMob::animalMob(int a, int b)
{
	target = NULL;
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
	mobType = 5;
	health = 10;
	ping = 0;
	animation = 0;
	notarget = 0;
	timeTillWifiUpdate = rand() % 4 + 4;
	smallmob = true;
	animal = AnimalType(rand() % 3);
}

void animalMob::updateMob(WorldObject* world)
{
	if (animation == 0) showGraphic(&animalGraphics[animal][0], x - world->camX - 7, y - world->camY - 7, facing ? true : false);
	else if (animation == 1) showGraphic(&animalGraphics[animal][1], x - world->camX - 7, y - world->camY - 7, facing ? true : false);

	if (host == true)
	{
		target = mobHandlerFindMob(256, 2, x, y);

		if (scaredTimer == 1)
		{
			vx = 0;
			scaredTimer = 0;
		}

		if (target->mobType != 2) //Out of range of player
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
		if (mobType == 2) notarget = 0;
		ping = 0;
		if (health <= 0)
			killMob();
		//if (notarget > 1800) killMob();
		if (animationClearFrames == 0) animation = 0;
		else --animationClearFrames;
	}
}

void animalMob::sendWifiUpdate()
{
}

void animalMob::saveToFile(FILE* pFile)
{
}

void animalMob::loadFromFile(FILE* pFile)
{
	killMob();
}

void animalMob::hurt(int amount, int type)
{

	if (animation == 1)
		return;
	if (jumpHurtType(type) && collisions[SIDE_BOTTOM])
		vy = JUMP_VELOCITY;
	health -= amount;
	switch (animal)
	{
		case ANIMAL_COW:
			playSound(SOUND_COW_HURT);
			break;
		case ANIMAL_SHEEP:
			playSound(SOUND_SHEEP_HURT);
			break;
		case ANIMAL_PIG:
			playSound(SOUND_PIG_HURT);
			break;
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
	animation = 1;
	animationClearFrames = 20;
}

bool animalMob::isMyPlayer()
{
	return false;
}

bool canAnimalMobSpawnHere(WorldObject* world, int x, int y)
{
	return canMobSpawnHere(world, x, y);
}

void animalMobInit()
{
	loadGraphic(&animalGraphics[ANIMAL_PIG][0], true, 10, 16, 16);
	loadGraphic(&animalGraphics[ANIMAL_PIG][1], true, 11, 16, 16);
	loadGraphic(&animalGraphics[ANIMAL_COW][0], true, 12, 16, 16);
	loadGraphic(&animalGraphics[ANIMAL_COW][1], true, 13, 16, 16);
	loadGraphic(&animalGraphics[ANIMAL_SHEEP][0], true, 14, 16, 16);
	loadGraphic(&animalGraphics[ANIMAL_SHEEP][1], true, 15, 16, 16);
}