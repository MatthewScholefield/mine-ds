#include "world.h"
#include <cstdlib>
#include "blockID.h"
#include "blocks.h"

int findFirstBlock(worldObject* world, int x)
{
	int i;
	for (i = 0; i <= WORLD_HEIGHT; ++i)
		if (!isBlockWalkThrough(world->blocks[x][i])) return i;
	return -1;
}

/*int findFirstBiomeBlock(worldObject* world,int x)
{
	int i;
	for (i=0; i<=WORLD_HEIGHT; ++i)
		if (!isAGroundBlock(world->blocks[x][i])) return i;
	return -1;
}*/
void drawLineDown(worldObject* world, int x, int y)
{
	int i;
	for (i = y; i < WORLD_HEIGHT; ++i)
	{
		if (i > WORLD_HEIGHT - 20 && rand() % 120 == 1) //20 blocks from bottom
			world->blocks[x][i] = DIAMOND_ORE;
		else if (i > WORLD_HEIGHT - 30 && rand() % 90 == 1) //30 blocks from bottom
			world->blocks[x][i] = GOLD_ORE;
		else if (i > WORLD_HEIGHT - 65 && rand() % 80 == 1) //65 blocks from bottom
			world->blocks[x][i] = IRON_ORE;
		else if (i > WORLD_HEIGHT - 80 && rand() % 60 == 1) //80 blocks from bottom
			world->blocks[x][i] = COAL_ORE;
		else world->blocks[x][i] = STONE;
	}
}

void drawLineThing(worldObject* world, int x1, int y1, int x2, int y2)
{
	// if x1 == x2 or y1 == y2, then it does not matter what we set here
	int delta_x(x2 - x1);
	signed char ix((delta_x > 0) - (delta_x < 0));
	delta_x = std::abs(delta_x) << 1;

	int delta_y(y2 - y1);
	signed char iy((delta_y > 0) - (delta_y < 0));
	delta_y = std::abs(delta_y) << 1;

	drawLineDown(world, x1, y1);

	if (delta_x >= delta_y)
	{
		// error may go below zero
		int error(delta_y - (delta_x >> 1));

		while (x1 != x2)
		{
			if (error >= 0)
			{
				if (error || (ix > 0))
				{
					y1 += iy;
					error -= delta_x;
				}
				// else do nothing
			}
			// else do nothing

			x1 += ix;
			error += delta_y;

			drawLineDown(world, x1, y1);
		}
	}
	else
	{
		// error may go below zero
		int error(delta_x - (delta_y >> 1));

		while (y1 != y2)
		{
			if (error >= 0)
			{
				if (error || (iy > 0))
				{
					x1 += ix;
					error -= delta_y;
				}
				// else do nothing
			}
			// else do nothing

			y1 += iy;
			error += delta_x;

			drawLineDown(world, x1, y1);
		}
	}
}
