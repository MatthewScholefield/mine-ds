#include <nds.h>
#include <stdio.h>
#include "BaseMob.h"
#include "../collision.h"
#include "../blocks.h"
#include "mobCollisions.h"
#include "../general.h"
#include "mobFunctions.h"
#include "hurt.h"
#include "../graphics/particleHandler.h"
#include "../blockUpdaters/WaterUpdater.h"

void calculatePhysics(World &world,BaseMob *mob, bool inWater)
{
	if (mob->collisions[SIDE_BOTTOM] && mob->vy > 0)
	{
		if (int(mob->vy) > 9)
			mob->hurt(2 * (mob->vy - 9), FALL_HURT);
		mob->vy = 0;
	}
	else if (mob->collisions[SIDE_TOP] && mob->vy < 0)
		mob->vy = 0;
	else
		mob->vy += FixedPoint(true, ((inWater ? 6 : 18) * FixedPoint::SCALER) / FPS); //Gravity Acceleration = +18.0 m/s^2

	int velocityCap = 25;
	if (inWater)
	{
		velocityCap = 2;
		//Allow the velocity to be less than -2 only if there is air above the player.
		if (mob->vy<-2) mob->vy = -2;
		if (abs(mob->vx) > 2) mob->vx = 2 * (mob->vx > 0 ? 1 : -1);
	}
	//Don't remove the y velocity so quickly.
	if (mob->vy > velocityCap) mob->vy -= (mob->vy - velocityCap) / 4;
}

int blockAtPixel(World &world, int pixX, int pixY)
{
	return world.blocks[pixX / 16][pixY / 16];
}

void calculateCollisions(World &world, BaseMob *mob)
{
	mob->collisions[SIDE_BOTTOM] = !isBlockWalkThrough(world.blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y + mob->sy / 2) / 16])
			|| !isBlockWalkThrough(world.blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y + mob->sy / 2) / 16]);
	mob->collisions[SIDE_RIGHT] = !isBlockWalkThrough(world.blocks[int((mob->x + mob->sx / 2 + 1) / 16)][int((mob->y - mob->sy / 2 + 1) / 16)])
			|| !isBlockWalkThrough(world.blocks[int((mob->x + mob->sx / 2 + 1) / 16)][int((mob->y) / 16)])
			|| !isBlockWalkThrough(world.blocks[int((mob->x + mob->sx / 2 + 1) / 16)][int((mob->y + mob->sy / 2 - 1) / 16)]);
	mob->collisions[SIDE_LEFT] = !isBlockWalkThrough(world.blocks[int((mob->x - mob->sx / 2) / 16)][int((mob->y - mob->sy / 2 + 1) / 16)])
			|| !isBlockWalkThrough(world.blocks[int((mob->x - mob->sx / 2) / 16)][int((mob->y) / 16)])
			|| !isBlockWalkThrough(world.blocks[int((mob->x - mob->sx / 2) / 16)][int((mob->y + mob->sy / 2 - 1) / 16)]);
	mob->collisions[SIDE_TOP] = !isBlockWalkThrough(world.blocks[int((mob->x - mob->sx / 2 + 1) / 16)][int((mob->y - mob->sy / 2) / 16)])
			|| !isBlockWalkThrough(world.blocks[int((mob->x + mob->sx / 2) / 16)][int((mob->y - mob->sy / 2) / 16)]);
	mob->collisions[COLLISION_STUCK] = false;
}

static void calcGeneralData(World &world, BaseMob *mob)
{ //For both small and large mobs
	bool wasInWater = mob->isInWater;
	mob->isInWater = isWaterAt(world, mob->x, mob->y + mob->sy / 2 - 2) || isWaterAt(world, mob->x, mob->y - mob->sy / 2 + 1);
	if (wasInWater == false && mob->isInWater)
		createSplash(mob->x, mob->y, mob->vx, mob->vy);
	calculatePhysics(world, mob, mob->isInWater);
	for (int b = -1; b <= 1; ++b)
		cactusCheck(world, mob, (mob->x) / 16, (mob->y) / 16 + b);
}

void calculateMiscData(World &world, BaseMob *mob)
{
	if (mob->host)
	{
		calcGeneralData(world, mob);
		if (mob->isInBlock(world))
		{ //move player out of block
			if ((!isBlockWalkThrough(world.blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y) / 16])
					&& isBlockWalkThrough(world.blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y) / 16]))
					||
					(!isBlockWalkThrough(world.blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y - mob->sy / 2 + 1) / 16])
					&& isBlockWalkThrough(world.blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y - mob->sy / 2 + 1) / 16]))
					||
					(!isBlockWalkThrough(world.blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y + mob->sy / 2 - 1) / 16])
					&& isBlockWalkThrough(world.blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y + mob->sy / 2 - 1) / 16])))
				mob->x -= int(mob->x + mob->sx / 2) % 16 + 1;
			else if ((!isBlockWalkThrough(world.blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y) / 16])
					&& isBlockWalkThrough(world.blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y) / 16]))
					||
					(!isBlockWalkThrough(world.blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y - mob->sy / 2 + 1) / 16])
					&& isBlockWalkThrough(world.blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y - mob->sy / 2 + 1) / 16]))
					||
					(!isBlockWalkThrough(world.blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y + mob->sy / 2 - 1) / 16])
					&& isBlockWalkThrough(world.blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y + mob->sy / 2 - 1) / 16])))
				mob->x += 16 - int(mob->x - mob->sx / 2 + 1) % 16;
			else if (int(mob->y) % 16 < 9)
				mob->y -= int(mob->y) % 16;
		}

		calculateCollisions(world, mob);
		if ((!mob->collisions[SIDE_BOTTOM] && mob->vy > 0) || (!mob->collisions[SIDE_TOP] && mob->vy < 0))
			mob->y += (16 * mob->vy) / FPS; //Main statement Positive velocity=down
		if ((mob->vx > 0 && !mob->collisions[SIDE_RIGHT]) || (mob->vx < 0 && !mob->collisions[SIDE_LEFT]))
			mob->x += (16 * mob->vx) / FPS;
		if (mob->x < mob->sx / 2) mob->x = mob->sx / 2;
		if (mob->y < mob->sy / 2) mob->y = mob->sy / 2;
		if (mob->x > World::WIDTH * World::BLOCK_PX - mob->sx / 2) mob->x = World::WIDTH * World::BLOCK_PX - mob->sx / 2;
	}
}

void calculateMiscDataSmall(World &world, BaseMob *mob)
{
	if (mob->host)
	{
		calcGeneralData(world, mob);
		if (mob->isInBlock(world))
		{ //move mob out of block
			if ((!isBlockWalkThrough(world.blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y) / 16])
					&& isBlockWalkThrough(world.blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y) / 16]))
					||
					(!isBlockWalkThrough(world.blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y + mob->sy / 2 - 1) / 16])
					&& isBlockWalkThrough(world.blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y + mob->sy / 2 - 1) / 16])))
				mob->x -= int(mob->x + mob->sx / 2) % 16 + 1;
			else if ((!isBlockWalkThrough(world.blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y) / 16])
					&& isBlockWalkThrough(world.blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y) / 16]))
					||
					(!isBlockWalkThrough(world.blocks[int(mob->x - mob->sx / 2 + 1) / 16][int(mob->y + mob->sy / 2 - 1) / 16])
					&& isBlockWalkThrough(world.blocks[int(mob->x + mob->sx / 2) / 16][int(mob->y + mob->sy / 2 - 1) / 16])))
				mob->x += 16 - int(mob->x - mob->sx / 2 + 1) % 16;
			else if (int(mob->y + mob->sy / 2) % 16 < 9)
				mob->y -= int(mob->y + mob->sy / 2) % 16;
		}

		calculateCollisions(world, mob);
		if ((!mob->collisions[SIDE_BOTTOM] && mob->vy > 0) || (!mob->collisions[SIDE_TOP] && mob->vy < 0))
			mob->y += (16 * mob->vy) / FPS; //Main statement Positive velocity=down
		if ((mob->vx > 0 && !mob->collisions[SIDE_RIGHT]) || (mob->vx < 0 && !mob->collisions[SIDE_LEFT]))
			mob->x += (16 * mob->vx) / FPS;

		if (mob->x < mob->sx / 2) mob->x = mob->sx / 2;
		if (mob->y < mob->sy / 2) mob->y = mob->sy / 2;
		if (mob->x > World::WIDTH * World::BLOCK_PX - mob->sx / 2) mob->x = World::WIDTH * World::BLOCK_PX - mob->sx / 2;
	}
}

bool onScreen(int x, int y, int camX, int camY)
{
	return x > camX && x < camX + 256 && y > camY && y < camY + 192;
}

