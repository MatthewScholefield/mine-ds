#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "jungleLeaf.h"
#include "leaf.h"

jungleLeafUpdater::jungleLeafUpdater()
{
	blockID = JUNGLE_LEAF;
	chance = LEAF_CHANCE_UPDATE;
}

void jungleLeafUpdater::chanceUpdate(worldObject* world, int x, int y, bool bg)
{
	leafChanceUpdateGeneric(world, x, y, bg);
}
