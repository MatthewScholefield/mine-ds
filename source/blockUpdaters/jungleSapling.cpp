#include "../world.h"
#include "../blockUpdater.h"
#include "../blockID.h"
#include "jungleSapling.h"
#include "oakSapling.h"

JungleSaplingUpdater::JungleSaplingUpdater()
{
	chance = SAPLING_CHANCE_UPDATE;
}

bool JungleSaplingUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	saplingUpdate(world, x, y, bg);
	return false;
}

void JungleSaplingUpdater::chanceUpdate(WorldObject &world, int x, int y, bool bg)
{
	saplingChanceUpdate(world, x, y, bg);
}