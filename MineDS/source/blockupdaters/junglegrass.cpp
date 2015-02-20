#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "junglegrass.h"

junglegrassUpdater::junglegrassUpdater()
{
  blockID = JUNGLE_GRASS;
	chance = 1000;
}
void junglegrassUpdater::chanceUpdate(worldObject* world, int x, int y, bool bg)
{
  if (bg == false)
  {
    if (isBlockWalkThrough(world->blocks[x][y-1]==false))
    {
      world->blocks[x][y]=DIRT;
    }
  }
  else
  {
    if (isBlockWalkThrough(world->blocks[x][y-1]==false) || isBlockWalkThrough(world->bgblocks[x][y-1]==false))
    {
      world->bgblocks[x][y]=DIRT;
    }
  }
}
