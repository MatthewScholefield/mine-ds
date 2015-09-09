#include <algorithm>
#include <vector>

#include "blockUpdater.h"
#include "world.h"
#include "blocks.h"
#include "blockID.h"
#include "nifi.h"
#include "communications.h"
#include "inventory.h"
#include "general.h"
#include "mobs/mobFunctions.h"
#include "graphics/3DHandler.h"

//Include blockUpdaters
#include "blockUpdaters/litFurnace.h"
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
#include "blockUpdaters/door.h"
#include "blockUpdaters/water.h"
#include "blockUpdaters/furnace.h"

//Todo: Profile to find out which of std::vector, std::list or std::forward_list is the quickest.
std::vector<BlockUpdateInfo> updaterList;
BlockUpdater* blockUpdaters[NUM_UPDATERS];

void proceduralBlockUpdateInit()
{
	blockUpdaters[0] = new LitFurnaceUpdater;
	blockUpdaters[1] = new AirUpdater;
	blockUpdaters[2] = new GrassUpdater;
	blockUpdaters[3] = new JungleGrassUpdater;
	blockUpdaters[4] = new MyceliumUpdater;
	blockUpdaters[5] = new DirtUpdater;
	blockUpdaters[6] = new LeafUpdater;
	blockUpdaters[7] = new RedwoodLeafUpdater;
	blockUpdaters[8] = new JungleLeafUpdater;
	blockUpdaters[9] = new SnowTopUpdater;
	blockUpdaters[10] = new SnowGrassUpdater;
	blockUpdaters[11] = new CactusUpdater;
	blockUpdaters[12] = new ShrubUpdater;
	blockUpdaters[13] = new TallGrassUpdater;
	blockUpdaters[14] = new RedFlowerUpdater;
	blockUpdaters[15] = new YellowFlowerUpdater;
	blockUpdaters[16] = new RedMushroomUpdater;
	blockUpdaters[17] = new BrownMushroomUpdater;
	blockUpdaters[18] = new OakSaplingUpdater;
	blockUpdaters[19] = new JungleSaplingUpdater;
	blockUpdaters[20] = new SpruceSaplingUpdater;
	blockUpdaters[21] = new LadderUpdater;
	blockUpdaters[22] = new DoorUpdater;
	blockUpdaters[23] = new DoorTopClosedUpdater;
	blockUpdaters[24] = new DoorTopOpenUpdater;
	blockUpdaters[25] = new DoorBottomClosedUpdater;
	blockUpdaters[26] = new DoorBottomOpenUpdater;
	blockUpdaters[27] = new WaterUpdater;
	blockUpdaters[28] = new FurnaceUpdater;
}

int updaterIndex(int blockID, int index = 0)
{
	switch (blockID)
	{
	case FURNACE_LIT: return 0;
	case AIR: return 1;
	case GRASS: return 2;
	case GRASS_JUNGLE: return 3;
	case MYCELIUM: return 4;
	case DIRT: return 5;
	case LEAVES_OAK: return 6;
	case LEAVES_SPRUCE: return 7;
	case LEAVES_JUNGLE: return 8;
	case SNOW_TOP: return 9;
	case GRASS_SNOW: return 10;
	case CACTUS: return 11;
	case SHRUB: return 12;
	case TALL_GRASS: return 13;
	case FLOWER_RED: return 14;
	case FLOWER_YELLOW: return 15;
	case MUSHROOM_RED: return 16;
	case MUSHROOM_BROWN: return 17;
	case SAPLING_OAK: return 18;
	case SAPLING_JUNGLE: return 19;
	case SAPLING_SPRUCE: return 20;
	case LADDER: return 21;
	case DOOR_ITEM: return 22;
	case DOOR_CLOSED_TOP: return 23;
	case DOOR_OPEN_TOP: return 24;
	case DOOR_CLOSED_BOTTOM: return 25;
	case DOOR_OPEN_BOTTOM: return 26;
	case WATER: return 27;
	case FURNACE: return 28;
	default:
		return -1;
	}
}

static void recursiveUpdate(WorldObject *world, int x, int y, bool bg);

void updateDir(WorldObject *world, int x, int y, bool bg, int dir)
{
	switch (dir)
	{
	case 0:
		recursiveUpdate(world, x + 1, y, bg);
		break;
	case 1:
		recursiveUpdate(world, x, y - 1, bg);
		break;
	case 2:
		recursiveUpdate(world, x - 1, y, bg);
		break;
	case 3:
		recursiveUpdate(world, x, y + 1, bg);
		break;
	default:
		break;
	}
}

void updateBlocksAround(WorldObject *world, int x, int y, bool bg)
{
	recursiveUpdate(world, x + 1, y, bg);
	recursiveUpdate(world, x, y - 1, bg);
	recursiveUpdate(world, x - 1, y, bg);
	recursiveUpdate(world, x, y + 1, bg);
	recursiveUpdate(world, x + 1, y + 1, bg);
	recursiveUpdate(world, x - 1, y - 1, bg);
	recursiveUpdate(world, x - 1, y + 1, bg);
	recursiveUpdate(world, x + 1, y - 1, bg);
	recursiveUpdate(world, x + 2, y, bg);
	recursiveUpdate(world, x, y - 2, bg);
	recursiveUpdate(world, x - 2, y, bg);
	recursiveUpdate(world, x, y + 2, bg);
	recursiveUpdate(world, x, y, bg);
}
bool findUpdateInfo(int x,int y, bool bg,bool chance)
{
	std::vector<BlockUpdateInfo>::iterator it;
	for (it = updaterList.begin(); it != updaterList.end(); it++)
	{
		if (it->x == x && it->y == y && it->bg == bg && it->chance == chance)
			return true;
	}
	return false;
}
void recursiveUpdate(WorldObject *world, int x, int y, bool bg)
{
	if (x < 0 || x > WORLD_WIDTH || y < 0 || y > WORLD_HEIGHT ) return;
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	int index = updaterIndex(blockXY);
	if (index!=-1)
	{
		if (!findUpdateInfo(x,y,bg,false))
			updaterList.push_back(BlockUpdateInfo{x, y, bg, 1, false});
		if (blockUpdaters[index]->chance!=NO_CHANCE && !findUpdateInfo(x,y,bg,true))
			updaterList.push_back(BlockUpdateInfo{x,y,bg,rand() % blockUpdaters[index]->chance,true});
			
	}
}


void updateAround(WorldObject *world, int x, int y)
{
	updateBlocksAround(world,x,y,false);
	updateBlocksAround(world,x,y,true);
}
void updateSingleBlock(WorldObject* world, int x, int y, bool bg, int timeToUpdate)
{
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	int index = updaterIndex(blockXY);
	if (index!=-1)
	{
		if (!findUpdateInfo(x,y,bg,false))
		{
			updaterList.push_back(BlockUpdateInfo{x,y,bg,timeToUpdate,false});
		}
	}
}
int processTTL(WorldObject* world)
{
	std::vector<BlockUpdateInfo>::iterator it;
	int numReadyToUpdate = 0;
	for (it = updaterList.begin(); it != updaterList.end(); ++it)
	{

		Color a = Color({255,0,0});
		it->TimeToLive-=1;
		if (it->TimeToLive < 2){
			numReadyToUpdate+=1;
			a = Color({0,255,0});
		}
		//drawRect(a, it->x * 16 + 4 - world->camX, it->y * 16 + 4 - world->camY, 8, 8);
	}
	return numReadyToUpdate;
}
void processOneBlock(WorldObject* world)
{
std::vector<BlockUpdateInfo>::iterator it;
	for (it = updaterList.begin(); it != updaterList.end(); ++it)
	{

		if (it->TimeToLive<2)
		{
			int x = it->x;
 			int y = it->y;
			bool bg = it->bg;
			bool chance = it->chance;
			it = updaterList.erase(it);
			if (x >= 0 && x < WORLD_WIDTH && y >= 0 && y < WORLD_HEIGHT)
			{
				int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
				int index = updaterIndex(blockXY);
				if (index!=-1)
				{
					if(!chance)
					{
						if(blockUpdaters[index]->update(world,x,y,bg))
							updateAround(world,x,y);
					}
					else
					{
						blockUpdaters[index]->chanceUpdate(world,x,y,bg);
					}
				}
			}
			break;
		}
	}
}
void proceduralBlockUpdate(WorldObject* world)
{
	if (updaterList.size()==0) return;
	//Go through the UpdaterList, and update if the TTL is 1, call updateAround if the update requests it.
	//If TTL is not 1, decrement the TTL value.
	int amount = processTTL(world);
	int t = 0;
	if (amount > 0 && amount < 5) t = amount;
	else if (amount > 4 && amount < 20 ) t = amount / 2;
	else t = 10;
	for (int i = 0; i < t; ++i) processOneBlock(world);
}
