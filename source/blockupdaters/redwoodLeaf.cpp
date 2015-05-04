#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "redwoodLeaf.h"
#include "leaf.h"

redwoodLeafUpdater::redwoodLeafUpdater()
{
	blockID = LEAF_SPRUCE;
	chance = LEAF_CHANCE_UPDATE;
}

void redwoodLeafUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg)
{
	leafChanceUpdateGeneric(world, x, y, bg);
}