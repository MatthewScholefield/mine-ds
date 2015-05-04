#include "../world.h"
#include "../blockupdate.h"
#include "../blockID.h"
#include "jungleSapling.h"
#include "oakSapling.h"

jungleSaplingUpdater::jungleSaplingUpdater()
{
	blockID = SAPLING_JUNGLE;
	chance = SAPLING_CHANCE_UPDATE;
}

void jungleSaplingUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	saplingUpdate(world, x, y, bg);
}

void jungleSaplingUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg)
{
	saplingChanceUpdate(world, x, y, bg);
}