#include "../world.h"
#include "../blockupdate.h"
#include "../blockID.h"
#include "spruceSapling.h"
#include "oakSapling.h"

spruceSaplingUpdater::spruceSaplingUpdater()
{
	blockID = SAPLING_SPRUCE;
	chance = SAPLING_CHANCE_UPDATE;
}

void spruceSaplingUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	saplingUpdate(world, x, y, bg);
}

void spruceSaplingUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg)
{
	saplingChanceUpdate(world, x, y, bg);
}
