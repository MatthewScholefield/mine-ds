#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include "../mobs/mobHandler.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "../mobs/hurt.h"
#include "leaf.h"
#include "../mainGame.h"

void leafChanceUpdateGeneric(worldObject* world, int x, int y, bool bg)
{
	int blockID = world->bgblocks[x][y];
	bool nearWood = false;
	for (int i = x - LEAF_SQUARE_RADIUS; i < x + LEAF_SQUARE_RADIUS; ++i)
	{
		for (int j = y - LEAF_SQUARE_RADIUS; j < y + LEAF_SQUARE_RADIUS; ++j)
			if (getType(bg ? world->bgblocks[i][j] : world->blocks[i][j]) == WOOD)
			{
				nearWood = true;
				break;
			}
		if (nearWood)
			break;
	}
	if (!nearWood && bg)
		world->bgblocks[x][y] = AIR;
	else if (!nearWood)
		world->blocks[x][y] = AIR;
  if (!nearWood && (rand() % 5)==0 && isSurvival())
  {
    //Spawn an item mob, and send blockId and amount.
    int mobNum = spawnMobAt(8,world,x*16+rand()%8,y*16);
    mobHandlerHurtMob(mobNum,blockID,PROPERTY_HURT);
	mobHandlerHurtMob(mobNum,1,PROPERTY_HURT);
  }
}

leafUpdater::leafUpdater()
{
	blockID = LEAF;
	chance = LEAF_CHANCE_UPDATE;
}

void leafUpdater::chanceUpdate(worldObject* world, int x, int y, bool bg)
{
	leafChanceUpdateGeneric(world, x, y, bg);
}
