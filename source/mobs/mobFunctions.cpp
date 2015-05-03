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
	if ((mob->collisions[0] && mob->vy > 0) || (mob->collisions[3] && mob->vy < 0) || mob->collisions[4])
		mob -> vy = 0;
	else
		mob->vy += (9.8 / 60.0);

	//Velocity Cap
	if (mob->vy > 10) mob->vy = 10;
}

int blockAtPixel(worldObject *world, int pixX, int pixY)
{
	return world->blocks[pixX / 16][pixY / 16];
}

void calculateMiscData(worldObject* world, baseMob* mob)
{
	if (mob->host)
	{
		for (int b = -1; b <= 1; ++b)
			cactusCheck(world, mob, 0, (mob->x) / 16, (mob->y) / 16 + b, false);
		mob->y += int(mob->vy); //Main statement Positive velocity=down

		//Create Collision Points according to diagram
		int *xPoints[6];
		int *yPoints[6];
		xPoints[0] = new int(mob->x - mob->sx / 2 + 1);
		yPoints[0] = new int(mob->y - mob->sy / 2 + 1);
		xPoints[1] = new int(mob->x + mob->sx / 2);
		yPoints[1] = new int(mob->y - mob->sy / 2 + 1);
		xPoints[2] = new int(mob->x - mob->sx / 2 + 1);
		yPoints[2] = new int(mob->y);
		xPoints[3] = new int(mob->x + mob->sx / 2);
		yPoints[3] = new int(mob->y);
		xPoints[4] = new int(mob->x - mob->sx / 2 + 1);
		yPoints[4] = new int(mob->y + mob->sy / 2 - 1);
		xPoints[5] = new int(mob->x + mob->sx / 2);
		yPoints[5] = new int(mob->y + mob->sy / 2 - 1);

		bool inBlock[6]; //Check whether each point is inside a block
		for (int i = 0; i < 6; ++i)
			inBlock[i] = !isBlockWalkThrough(world->blocks[*xPoints[i] / 16][*yPoints[i] / 16]);

		if (inBlock[4]&&!inBlock[2])
			mob->y = *yPoints[2] - *yPoints[2] % 16 + 16 - mob->sy / 2;
		else if (inBlock[5]&&!inBlock[3])
			mob->y = *yPoints[3] - *yPoints[3] % 16 + 16 - mob->sy / 2;
		else if (inBlock[0] && !inBlock[2])
			mob->y = *yPoints[2]-*yPoints[2] % 16 + mob->sy / 2 - 1;
		else if (inBlock[1] && !inBlock[3])
			mob->y = *yPoints[3]-*yPoints[3] % 16 + mob->sy / 2 - 1;
		else
		{
			for (int i = 0; i < 6; i += 2)
				if (inBlock[i] != inBlock[i + 1])
				{
					mob->x = inBlock[i] ? (*xPoints[i + 1] - (*xPoints[i + 1] % 16) + abs(*xPoints[i] - mob->x))
							: (*xPoints[i] - (*xPoints[i] % 16) + 16 - abs(*xPoints[i + 1] - mob->x));
					mob->vx = 0;
				}
		}

		mob->collisions[0] = !isBlockWalkThrough(world->blocks[int((mob->x - mob->sx / 2 + 1) / 16)][int((mob->y + mob->sy / 2) / 16)])
				|| !isBlockWalkThrough(world->blocks[int((mob->x + mob->sx / 2) / 16)][int((mob->y + mob->sy / 2) / 16)]);
		mob->collisions[1] = !isBlockWalkThrough(world->blocks[int((mob->x + mob->sx / 2 + 1) / 16)][int((mob->y - mob->sy / 2 + 1) / 16)])
				|| !isBlockWalkThrough(world->blocks[int((mob->x + mob->sx / 2 + 1) / 16)][int((mob->y) / 16)])
				|| !isBlockWalkThrough(world->blocks[int((mob->x + mob->sx / 2 + 1) / 16)][int((mob->y + mob->sy / 2 - 1) / 16)]);
		mob->collisions[2] = !isBlockWalkThrough(world->blocks[int((mob->x - mob->sx / 2) / 16)][int((mob->y - mob->sy / 2 + 1) / 16)])
				|| !isBlockWalkThrough(world->blocks[int((mob->x - mob->sx / 2) / 16)][int((mob->y) / 16)])
				|| !isBlockWalkThrough(world->blocks[int((mob->x - mob->sx / 2) / 16)][int((mob->y + mob->sy / 2 - 1) / 16)]);
		mob->collisions[3] = !isBlockWalkThrough(world->blocks[int((mob->x - mob->sx / 2 + 1) / 16)][int((mob->y - mob->sy / 2) / 16)])
				|| !isBlockWalkThrough(world->blocks[int((mob->x + mob->sx / 2) / 16)][int((mob->y - mob->sy / 2) / 16)]); 
		mob->collisions[4] = false;

		for (int i = 0; i < 6; ++i)
		{
			delete xPoints[i];
			delete yPoints[i];
		}

		if (mob->x < 1) mob->x = 1;
		if (mob->y < 1) mob->y = 1;
		calculatePhysics(mob);
	}
}

void calculateMiscDataSmall(worldObject* world, baseMob* mob)
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