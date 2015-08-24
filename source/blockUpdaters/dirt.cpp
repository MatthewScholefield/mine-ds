#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "dirt.h"

//The values to add to the x y coord for checking adjacent blocks
//Setting the evaluting block as the center, the scan coords are:
//(-1,1) (-1,0) (-1,-1) (1,1) (1,0) (1,-1)
const int scanAdd[][6] = {
	{-1, -1, -1, 1, 1, 1},
	{1, 0, -1, 1, 0, -1}
};

//(-1,1) requires that (-1,-1) is clear (For the grass to spread) etc
//(-1,0) doesn't require anything extra so although redundant,
//the check coord is the block above (-1,-1)
const int scanCheck[][6] = {
	{-1, -1, 0, 1, 1, 0},
	{-1, -1, -2, -1, -1, -2}
};

DirtUpdater::DirtUpdater()
{
	chance = SOIL_CHANCE_UPDATE;
}

//This function recursively checks for the ID of a grass block that can spread to the xy coord
//If it finds nothing, -1 is returned

int getGrassSpreadID(WorldObject* world, int x, int y, bool bg, int scanIndex = 0)
{ //scanIndex is the index of the scan arrays for the current
	if (!bg)
	{
		if (scanIndex >= 6)
			return -1;
		int blockID = world->blocks[x + scanAdd[0][scanIndex]][y + scanAdd[1][scanIndex]];

		//First checks if the block is a grass block
		//Next checks if the block above is open
		//Last checks if the corner block is open
		if (isGrassBlock(blockID)
				&& isBlockWalkThrough(world->blocks[x + scanAdd[0][scanIndex]][-1 + y + scanAdd[1][scanIndex]])
				&& isBlockWalkThrough(world->blocks[x + scanCheck[0][scanIndex]][y + scanCheck[1][scanIndex]]))
			return blockID;
		else
			return getGrassSpreadID(world, x, y, bg, scanIndex + 1);
	}
	else
	{
		if (scanIndex >= 6)
			return -1;
		int blockID = world->bgblocks[x + scanAdd[0][scanIndex]][y + scanAdd[1][scanIndex]];
		if (isGrassBlock(blockID)
				&& isBlockWalkThrough(world->bgblocks[x + scanAdd[0][scanIndex]][-1 + y + scanAdd[1][scanIndex]])
				&& isBlockWalkThrough(world->bgblocks[x + scanCheck[0][scanIndex]][y + scanCheck[1][scanIndex]]))
			return blockID;
		else
			return getGrassSpreadID(world, x, y, bg, scanIndex + 1);
	}
}

void DirtUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg)
{
	if (!bg)
	{
		if (!isBlockWalkThrough(world->blocks[x][y - 1]))
			return;
		int grassType = getGrassSpreadID(world, x, y, bg);
		if (grassType < 0)
			return;
		else
			world->blocks[x][y] = grassType;
		if (world->blocks[x][y] == SNOW_GRASS)
			world->blocks[x][y - 1] = SNOW_TOP;
	}
	else
	{
		if (isGrassBlock(world->blocks[x][y])) //Spread from foreground to background
			world->bgblocks[x][y] = world->blocks[x][y];
		if (!isBlockWalkThrough(world->bgblocks[x][y - 1]))
			return;
		int grassType = getGrassSpreadID(world, x, y, bg);
		if (grassType < 0)
			return;
		else
			world->bgblocks[x][y] = grassType;
		if (world->bgblocks[x][y] == SNOW_GRASS && isBlockWalkThrough(world->blocks[x][y]))
			world->bgblocks[x][y - 1] = SNOW_TOP;
	}
}