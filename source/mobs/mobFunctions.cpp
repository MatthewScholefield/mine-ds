#include <nds.h>
#include <stdio.h>
#include "baseMob.h"
#include "../collision.h"
#include "../blocks.h"
#include "mobCollisions.h"
#include "../general.h"
#include "mobFunctions.h"

void calculatePhysics(baseMob* mob)
{
	if ((!mob->collisions[0] && mob->vy > 0) || (!mob->collisions[3] && mob->vy < 0))
		mob->y += 16.0 * mob->vy / double(FPS); //Main statement Positive velocity=down
	if ((mob->vx > 0 && !mob->collisions[1]) || (mob->vx < 0 && !mob->collisions[2]))
		mob->x += int(16.0 * mob->vx / double(FPS));

	//Velocity Cap
	if (mob->vy > 25) mob->vy = 25;

	if ((mob->collisions[0] && mob->vy > 0) || (mob->collisions[3] && mob->vy < 0) || mob->collisions[4])
		mob -> vy = 0;
	else
		mob->vy += (18.0 / 60.0);
}

int blockAtPixel(WorldObject *world, int pixX, int pixY)
{
	return world->blocks[pixX / 16][pixY / 16];
}

void calculateMiscData(WorldObject* world, baseMob* mob)
{
	if (mob->host)
	{
		if (mob->x < 1) mob->x = 1;
		if (mob->y < 1) mob->y = 1;
		calculatePhysics(mob);
		for (int b = -1; b <= 1; ++b)
			cactusCheck(world, mob, 0, (mob->x) / 16, (mob->y) / 16 + b, false);

		bool inBlock = false;
		int inX = 0, inY = 0;

		for (int x = mob->x - mob->sx / 2 + 1; x < mob->x + mob->sx / 2 + 1; x += mob->sx - 1)
		{
			for (int y = mob->y - mob->sy / 2 + 1; y < mob->y + mob->sy / 2; y += mob->sy / 2 - 1)
			{
				if (!isBlockWalkThrough(world->blocks[x / 16][y / 16]))
				{
					inBlock = true;
					inX = x;
					inY = y;
					break;
				}
			}
			if (inBlock)
				break;
		}

		if (mob->mobtype == 2)
		{
			printXY(1, 1, inBlock ? "IN " : "OUT");
			printXY(1, 2, inX);
			printXY(1, 3, inY);
		}
		if (inBlock)
		{ //move player out of block
			if (int(mob->y) % 16 < 3)
				mob->y -= int(mob->y) % 16;
			if ((!isBlockWalkThrough(world->blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y) / 16])
					&& isBlockWalkThrough(world->blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y) / 16]))
					||
					(!isBlockWalkThrough(world->blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y - mob->sy / 2 + 1) / 16])
					&& isBlockWalkThrough(world->blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y - mob->sy / 2 + 1) / 16]))
					||
					(!isBlockWalkThrough(world->blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y + mob->sy / 2 - 1) / 16])
					&& isBlockWalkThrough(world->blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y + mob->sy / 2 - 1) / 16])))
				mob->x -= int(mob->x + mob->sx / 2) % 16 + 1;
			else if ((!isBlockWalkThrough(world->blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y) / 16])
					&& isBlockWalkThrough(world->blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y) / 16]))
					||
					(!isBlockWalkThrough(world->blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y - mob->sy / 2 + 1) / 16])
					&& isBlockWalkThrough(world->blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y - mob->sy / 2 + 1) / 16]))
					||
					(!isBlockWalkThrough(world->blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y + mob->sy / 2 - 1) / 16])
					&& isBlockWalkThrough(world->blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y + mob->sy / 2 - 1) / 16])))
				mob->x += 16 - int(mob->x - mob->sx / 2 + 1) % 16;
			else if (int(mob->y) % 16 < 9)
				mob->y -= int(mob->y) % 16;
		}

		//Calculate collisions
		mob->collisions[0] = !isBlockWalkThrough(world->blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y + mob->sy / 2) / 16])
				|| !isBlockWalkThrough(world->blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y + mob->sy / 2) / 16]);
		mob->collisions[1] = !isBlockWalkThrough(world->blocks[int((mob->x + mob->sx / 2 + 1) / 16)][int((mob->y - mob->sy / 2 + 1) / 16)])
				|| !isBlockWalkThrough(world->blocks[int((mob->x + mob->sx / 2 + 1) / 16)][int((mob->y) / 16)])
				|| !isBlockWalkThrough(world->blocks[int((mob->x + mob->sx / 2 + 1) / 16)][int((mob->y + mob->sy / 2 - 1) / 16)]);
		mob->collisions[2] = !isBlockWalkThrough(world->blocks[int((mob->x - mob->sx / 2) / 16)][int((mob->y - mob->sy / 2 + 1) / 16)])
				|| !isBlockWalkThrough(world->blocks[int((mob->x - mob->sx / 2) / 16)][int((mob->y) / 16)])
				|| !isBlockWalkThrough(world->blocks[int((mob->x - mob->sx / 2) / 16)][int((mob->y + mob->sy / 2 - 1) / 16)]);
		mob->collisions[3] = !isBlockWalkThrough(world->blocks[int((mob->x - mob->sx / 2 + 1) / 16)][int((mob->y - mob->sy / 2) / 16)])
				|| !isBlockWalkThrough(world->blocks[int((mob->x + mob->sx / 2) / 16)][int((mob->y - mob->sy / 2) / 16)]);
		mob->collisions[4] = false;
	}
}

void calculateMiscDataSmall(WorldObject* world, baseMob* mob)
{
	/*if (mob->host)
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
			cactusCheck(world, mob, 0, blockx, blocky + b, false);
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
	}*/
	calculateMiscData(world, mob);
}

bool onScreen(int x, int y, int camX, int camY)
{
	return x > camX && x < camX + 256 && y > camY && y < camY + 192;
}