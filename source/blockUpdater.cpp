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
#include "blockUpdaters/door.h"
#include "blockUpdaters/water.h"
#include "mobs/mobFunctions.h"
#include "graphics/3DHandler.h"

int numRecursions, triggerX, triggerY;
std::vector<std::vector<bool>> hasUpdated;
std::vector<std::vector<bool>> hasToUpdate(WORLD_WIDTH + 1, std::vector<bool>(WORLD_HEIGHT + 1, false));
std::vector <std::vector<bool>> temp(WORLD_WIDTH + 1, std::vector<bool>(WORLD_HEIGHT + 1, false));
BlockUpdater* blockUpdaters[NUM_UPDATERS];

BlockUpdater::BlockUpdater() { }

bool BlockUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	return false;
}

void BlockUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg) { }

void proceduralBlockUpdateInit()
{
	blockUpdaters[0] = new FurnaceUpdater;
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
}

static int updaterIndex(int blockID, int index = 0)
{
	switch (blockID)
	{
	case FURNACE: return 0;
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
	default:
		return -1;
	}
}

static void recursiveUpdate(WorldObject *world, int x, int y, bool bg);

static void updateDir(WorldObject *world, int x, int y, bool bg, int dir)
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

static void updateBlocksAround(WorldObject *world, int x, int y, bool bg)
{
	recursiveUpdate(world, x + 1, y, bg);
	recursiveUpdate(world, x, y - 1, bg);
	recursiveUpdate(world, x - 1, y, bg);
	recursiveUpdate(world, x, y + 1, bg);
}

static void recursiveUpdate(WorldObject *world, int x, int y, bool bg)
{
	if (hasUpdated[x][y])
	{
		hasToUpdate[x][y] = true;
		return;
	}
	hasUpdated[x][y] = true;
	if (!onScreen(x * 16, y * 16, world->camX, world->camY))
		return;
	drawRect(Color({255, 0, 0}), x * 16 + 4 - world->camX, y * 16 + 4 - world->camY, 8, 8);
	if (updaterIndex(bg ? world->bgblocks[x][y] : world->blocks[x][y]) != -1 && blockUpdaters[updaterIndex(bg ? world->bgblocks[x][y] : world->blocks[x][y])]->update(world, x, y, bg))
		updateBlocksAround(world, x, y, bg);
}

void startUpdate(WorldObject *world, int x, int y, bool bg)
{
	hasUpdated.assign(WORLD_WIDTH + 1, std::vector<bool>(WORLD_HEIGHT + 1, false));
	updaterIndex(bg ? world->bgblocks[x][y] : world->blocks[x][y]) != -1 && blockUpdaters[updaterIndex(bg ? world->bgblocks[x][y] : world->blocks[x][y])]->update(world, x, y, bg);
	updateBlocksAround(world, x, y, bg);
}

void updateAround(WorldObject *world, int x, int y)
{
	startUpdate(world, x, y, true);
	startUpdate(world, x, y, false);
}

static void updateBlock(WorldObject *world, int x, int y, bool bg)
{
	int i = updaterIndex(bg ? world->bgblocks[x][y] : world->blocks[x][y]);
	if (i >= 0 && --blockUpdaters[i]->timer < 0)
	{
		blockUpdaters[i]->timer = rand() % blockUpdaters[i]->chance;
		blockUpdaters[i]->chanceUpdate(world, x, y, bg);
	}
}

void proceduralBlockUpdateCheck(WorldObject* world, int x, int y)
{
	updateBlock(world, x, y, true);
	updateBlock(world, x, y, false);
}

void proceduralBlockUpdate(WorldObject* world)
{
	const int EXTRA_FRAME = 1;
	bool startedUpdating = false;
	for (int x = std::max(0, world->camX / 16 - EXTRA_FRAME); x <= std::min(WORLD_WIDTH, world->camX / 16 + 256 / 16 + EXTRA_FRAME); ++x)
		for (int y = std::max(0, world->camY / 16 - EXTRA_FRAME); y <= std::min(WORLD_HEIGHT, world->camY / 16 + 192 / 16 + EXTRA_FRAME); ++y)
		{
			temp[x][y] = hasToUpdate[x][y];
			hasToUpdate[x][y] = false;
		}
	for (int x = std::max(0, world->camX / 16 - EXTRA_FRAME); x <= std::min(WORLD_WIDTH, world->camX / 16 + 256 / 16 + EXTRA_FRAME); ++x)
		for (int y = std::max(0, world->camY / 16 - EXTRA_FRAME); y <= std::min(WORLD_HEIGHT, world->camY / 16 + 192 / 16 + EXTRA_FRAME); ++y)
		{
			if (temp[x][y])
			{
				if (!startedUpdating)
					startUpdate(world, x, y, false);
				else
					recursiveUpdate(world, x, y, false);
			}
			proceduralBlockUpdateCheck(world, x, y);
		}
}