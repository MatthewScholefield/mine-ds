#include "../world/WorldObject.h"
#include "blockUpdater.h"
#include "../blockID.h"
#include "JungleSaplingUpdater.h"
#include "OakSaplingUpdater.h"

JungleSaplingUpdater::JungleSaplingUpdater()
{
	chance = SAPLING_CHANCE_UPDATE;
}

bool JungleSaplingUpdater::update(World &world, int x, int y, bool bg)
{
	saplingUpdate(world, x, y, bg);
	return false;
}

void JungleSaplingUpdater::chanceUpdate(World &world, int x, int y, bool bg)
{
	saplingChanceUpdate(world, x, y, bg);
}