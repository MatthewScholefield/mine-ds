#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "../blocks.h"
#include "JungleLeafUpdater.h"
#include "OakLeafUpdater.h"

JungleLeafUpdater::JungleLeafUpdater()
{
	chance = LEAF_CHANCE_UPDATE;
}

void JungleLeafUpdater::chanceUpdate(WorldObject &world, int x, int y, bool bg)
{
	leafChanceUpdateGeneric(world, x, y, bg);
}
