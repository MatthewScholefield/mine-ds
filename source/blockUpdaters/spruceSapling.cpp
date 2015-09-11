#include "../world.h"
#include "../blockUpdater.h"
#include "../blockID.h"
#include "spruceSapling.h"
#include "oakSapling.h"

SpruceSaplingUpdater::SpruceSaplingUpdater()
{
	chance = SAPLING_CHANCE_UPDATE;
}

bool SpruceSaplingUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	saplingUpdate(world, x, y, bg);
	return false;
}

void SpruceSaplingUpdater::chanceUpdate(WorldObject &world, int x, int y, bool bg)
{
	saplingChanceUpdate(world, x, y, bg);
}
