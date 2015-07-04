#include "blockUpdater.h"
#include "world.h"
#include "blocks.h"
#include "blockID.h"
#include "nifi.h"
#include "communications.h"
#include "inventory.h"
#include "general.h"

//Include blockUpdaters
#include "blockUpdaters/furnace.h"
#include "blockUpdaters/air.h"
#include "blockUpdaters/grass.h"
#include "blockUpdaters/dirt.h"
#include "blockUpdaters/junglegrass.h"
#include "blockUpdaters/mycelium.h"
#include "blockUpdaters/leaf.h"
#include "blockUpdaters/redwoodLeaf.h"
#include "blockUpdaters/jungleLeaf.h"
#include "blockUpdaters/snowTop.h"
#include "blockUpdaters/snowGrass.h"
#include "blockUpdaters/cactus.h"
#include "blockUpdaters/plants.h"
#include "blockUpdaters/oakSapling.h"
#include "blockUpdaters/spruceSapling.h"
#include "blockUpdaters/jungleSapling.h"
#include "blockUpdaters/ladder.h"

BlockUpdater* blockUpdaters[50];
int numBlockUpdaters;

BlockUpdater::BlockUpdater()
{
}

void BlockUpdater::update(WorldObject* world, int x, int y, bool bg)
{
}

void BlockUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg)
{
}

void proceduralBlockUpdateInit()
{
	numBlockUpdaters = 0;
	blockUpdaters[numBlockUpdaters++] = new FurnaceUpdater;
	blockUpdaters[numBlockUpdaters++] = new AirUpdater;
	blockUpdaters[numBlockUpdaters++] = new GrassUpdater;
	blockUpdaters[numBlockUpdaters++] = new JungleGrassUpdater;
	blockUpdaters[numBlockUpdaters++] = new MyceliumUpdater;
	blockUpdaters[numBlockUpdaters++] = new DirtUpdater;
	blockUpdaters[numBlockUpdaters++] = new LeafUpdater;
	blockUpdaters[numBlockUpdaters++] = new RedwoodLeafUpdater;
	blockUpdaters[numBlockUpdaters++] = new JungleLeafUpdater;
	blockUpdaters[numBlockUpdaters++] = new SnowTopUpdater;
	blockUpdaters[numBlockUpdaters++] = new SnowGrassUpdater;
	blockUpdaters[numBlockUpdaters++] = new CactusUpdater;
	blockUpdaters[numBlockUpdaters++] = new ShrubUpdater;
	blockUpdaters[numBlockUpdaters++] = new TallGrassUpdater;
	blockUpdaters[numBlockUpdaters++] = new RedFlowerUpdater;
	blockUpdaters[numBlockUpdaters++] = new YellowFlowerUpdater;
	blockUpdaters[numBlockUpdaters++] = new RedMushroomUpdater;
	blockUpdaters[numBlockUpdaters++] = new BrownMushroomUpdater;
	blockUpdaters[numBlockUpdaters++] = new OakSaplingUpdater;
	blockUpdaters[numBlockUpdaters++] = new JungleSaplingUpdater;
	blockUpdaters[numBlockUpdaters++] = new SpruceSaplingUpdater;
	blockUpdaters[numBlockUpdaters++] = new LadderUpdater;
}

void proceduralBlockUpdateCheck(WorldObject* world, int x, int y)
{
	for (int i = 0; i < numBlockUpdaters; ++i)
	{
		if (world->bgblocks[x][y] == blockUpdaters[i]->blockID)
		{
			blockUpdaters[i]->update(world, x, y, true);
			if (!(rand() % blockUpdaters[i]->chance))
			{
				blockUpdaters[i]->chanceUpdate(world, x, y, true);
			}
		}
		if (world->blocks[x][y] == blockUpdaters[i]->blockID)
		{
			blockUpdaters[i]->update(world, x, y, false);
			if (!(rand() % blockUpdaters[i]->chance))
			{
				blockUpdaters[i]->chanceUpdate(world, x, y, false);
			}
		}
	}
}

void proceduralBlockUpdate(WorldObject* world)
{
	int sx = world->camX / 16;
	int sy = world->camY / 16;
	for (int x = sx; x < (sx + 16); ++x)
	{
		for (int y = sy; y < (sy + 12); ++y)
		{
			proceduralBlockUpdateCheck(world, x, y);
		}
	}
}
