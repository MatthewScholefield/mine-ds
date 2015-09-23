#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "redwoodLeaf.h"
#include "leaf.h"

RedwoodLeafUpdater::RedwoodLeafUpdater()
{
	chance = LEAF_CHANCE_UPDATE;
}

void RedwoodLeafUpdater::chanceUpdate(WorldObject &world, int x, int y, bool bg)
{
	leafChanceUpdateGeneric(world, x, y, bg);
}