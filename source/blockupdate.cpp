#include "blockupdate.h"
#include "world.h"
#include "blocks.h"
#include "blockID.h"
#include "nifi.h"
#include "communications.h"
#include "inventory.h"
#include "general.h"
//Include blockUpdaters
#include "blockupdaters/furnace.h"
#include "blockupdaters/air.h"
#include "blockupdaters/grass.h"
#include "blockupdaters/dirt.h"
#include "blockupdaters/junglegrass.h"
#include "blockupdaters/mycelium.h"
#include "blockupdaters/leaf.h"
#include "blockupdaters/redwoodLeaf.h"
#include "blockupdaters/jungleLeaf.h"
#include "blockupdaters/snowTop.h"
#include "blockupdaters/snowGrass.h"
#include "blockupdaters/cactus.h"
#include "blockupdaters/plants.h"
#include "blockupdaters/oakSapling.h"

blockUpdater* blockUpdaters[10];
int numBlockUpdaters;

blockUpdater::blockUpdater()
{
}

void blockUpdater::update(worldObject* world, int x, int y, bool bg)
{
}

void blockUpdater::chanceUpdate(worldObject* world, int x, int y, bool bg)
{
}

void proceduralBlockUpdateInit()
{
	numBlockUpdaters = 0;
	blockUpdaters[numBlockUpdaters++] = new furnaceUpdater;
	blockUpdaters[numBlockUpdaters++] = new airUpdater;
	blockUpdaters[numBlockUpdaters++] = new grassUpdater;
	blockUpdaters[numBlockUpdaters++] = new junglegrassUpdater;
	blockUpdaters[numBlockUpdaters++] = new myceliumUpdater;
	blockUpdaters[numBlockUpdaters++] = new dirtUpdater;
	blockUpdaters[numBlockUpdaters++] = new leafUpdater;
	blockUpdaters[numBlockUpdaters++] = new redwoodLeafUpdater;
	blockUpdaters[numBlockUpdaters++] = new jungleLeafUpdater;
	blockUpdaters[numBlockUpdaters++] = new snowTopUpdater;
	blockUpdaters[numBlockUpdaters++] = new snowGrassUpdater;
	blockUpdaters[numBlockUpdaters++] = new cactusUpdater;
	blockUpdaters[numBlockUpdaters++] = new shrubUpdater;
	blockUpdaters[numBlockUpdaters++] = new tallGrassUpdater;
	blockUpdaters[numBlockUpdaters++] = new redFlowerUpdater;
	blockUpdaters[numBlockUpdaters++] = new yellowFlowerUpdater;
	blockUpdaters[numBlockUpdaters++] = new redMushroomUpdater; 
	blockUpdaters[numBlockUpdaters++] = new brownMushroomUpdater;
	blockUpdaters[numBlockUpdaters++] = new oakSaplingUpdater;
}

void proceduralBlockUpdateCheck(worldObject* world, int x, int y)
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

void proceduralBlockUpdate(worldObject* world)
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