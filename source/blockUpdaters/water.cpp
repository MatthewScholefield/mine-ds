#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include "../blockID.h"
#include "../blocks.h"
#include "water.h"
#include "../mobs/mobHandler.h"

WaterUpdater::WaterUpdater()
{
	blockID = WATER;
	chance = NO_CHANCE;
}

void WaterUpdater::update(WorldObject* world, int x, int y, bool bg)
{

}
