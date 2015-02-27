#include <nds.h>
#include <stdio.h>
#include "baseMob.h"
#include "../collision.h"
#include "../blocks.h"
#include "mobCollisions.h"
#include "../general.h"

void calculatePhysics(baseMob* mob)
{
	if (mob->collisions[0] || mob->collisions[4])
		mob -> vy = 0;
	else
		mob->vy = mob->vy - (9.8 / 60.0);

	//Velocity Cap
	//if (mob->vy > 6) mob->vy = 6;
}

void calculateMiscData(worldObject* world, baseMob* mob)
{
	if (mob->host)
	{
		mob->y -= int(mob->vy); //Main statement Positive velocity=up
		int blockx = (mob->x + mob -> sx / 2) / 16;
		int blocky = (mob->y + 6 + mob -> sy / 2) / 16;
		for (int i = 0; i < 4; ++i)
			mob -> collisions[i] = false;
		int a;

		if (!isBlockWalkThrough(world->blocks[blockx][blocky - 1]))
			mob -> collisions[3] = true;
		if (spritecol(mob -> x, mob -> y, (blockx + 1)*16, (blocky - 1)*16, mob -> sx, mob ->sy, 16, 48) && !(isBlockWalkThrough(world->blocks[blockx + 1][blocky]) && isBlockWalkThrough(world->blocks[blockx + 1][blocky - 1])))
			mob -> collisions[1] = true;
		if (spritecol(mob -> x, mob -> y, (blockx - 1)*16, (blocky - 1)*16, mob -> sx, mob ->sy, 16, 48) && !(isBlockWalkThrough(world->blocks[blockx - 1][blocky]) && isBlockWalkThrough(world->blocks[blockx - 1][blocky - 1])))
			mob -> collisions[2] = true;
		bool collide = false;
		for (int b = -1; b <= 1; ++b)
			collision(world, mob, 0, blockx, blocky + b, false);
		for (a = 0; a <= 3; ++a)
		{
			if (spritecol(mob -> x + (a - 1), mob -> y, (blockx - 1 + a)*16, (blocky + 1)*16, mob -> sx, mob ->sy, 16, 16) && !isBlockWalkThrough(world->blocks[blockx - 1 + a][blocky + 1]))
			{
				if (a == 0 && mob->collisions[2] == false && mob->vy <= 0.0) collide = true;
				else if (a == 1) collide = true;
				else if (a == 2 && mob->collisions[1] == false && mob->vy <= 0.0) collide = true;
			}
		}
		if (collide)
		{
			mob -> collisions[0] = true;
			mob -> y = (blocky - 1)*16 + 1;
		}
		if (!isBlockWalkThrough(world->blocks[blockx][blocky]))
		{
			mob -> y -= 16;
			mob -> vy = 0.0;
		}
		if (spritecol(mob -> x, mob -> y, blockx, blocky, mob -> sx, mob -> sy, 16, 16))
		{
			//Mob is inside the block
			mob -> collisions[4] = true;
			mob -> y = (blocky -1 )*16 + 1;
		}
		if (mob->x < 1) mob->x = 1;
		calculatePhysics(mob);
	}
}

void calculateMiscDataSmall(worldObject* world, baseMob* mob)
{
	if (mob->host)
	{
		mob->y -= int(mob->vy); //Main statement Positive velocity=up
		int blockx = (mob->x + mob -> sx / 2) / 16;
		int blocky = (mob->y + mob -> sy / 2) / 16;
		for (int i = 0; i < 4; ++i)
			mob -> collisions[i] = false;
		int a;
		if (!isBlockWalkThrough(world->blocks[blockx][blocky - 1]))
			mob -> collisions[3] = true;
		if (spritecol(mob -> x + 2, mob -> y, (blockx + 1)*16, (blocky - 1)*16 + 8, mob -> sx, mob ->sy, 16, 24) && !(isBlockWalkThrough(world->blocks[blockx + 1][blocky])))
			mob -> collisions[1] = true;
		if (spritecol(mob -> x - 2, mob -> y, (blockx - 1)*16, (blocky - 1)*16 + 8, mob -> sx, mob ->sy, 16, 24) && !(isBlockWalkThrough(world->blocks[blockx - 1][blocky])))
			mob -> collisions[2] = true;
		bool collide = false;
		for (int b = -1; b <= 1; ++b)
			collision(world, mob, 0, blockx, blocky + b, false);
		for (a = 0; a <= 3; ++a)
		{
			if (spritecol(mob -> x + (a - 1), mob -> y, (blockx - 1 + a)*16, (blocky + 1)*16, mob -> sx, mob ->sy, 16, 16) && !isBlockWalkThrough(world->blocks[blockx - 1 + a][blocky + 1]))
			{
				if (a == 0 && mob->collisions[2] == false && mob->vy <= 0.0) collide = true;
				else if (a == 1) collide = true;
				else if (a == 2 && mob->collisions[1] == false && mob->vy <= 0.0) collide = true;
			}
		}
		if (collide)
		{
			mob -> collisions[0] = true;
			mob -> y = (blocky)*16 + 1;
		}
		if (spritecol(mob -> x, mob -> y, blockx, blocky, mob -> sx, mob -> sy, 16, 16))
		{
			//Mob is inside the block
			mob -> collisions[4] = true;
			mob -> y = (blocky - 1)*16 + 1;
		}
		if (mob->x < 1) mob->x = 1;
		calculatePhysics(mob);
	}
}
