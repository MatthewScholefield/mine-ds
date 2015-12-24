#include "../WorldObject.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "AirUpdater.h"

AirUpdater::AirUpdater()
{
	chance = NO_CHANCE;
}

bool AirUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	if (bg == false)
	{
		//Remove the foregraound data (x&0 = 0)
		world.data[x][y] &= 0xFFFF0000;
	}
	else
	{
		//Remove the background data (x&0 = 0)
		world.data[x][y] &= 0x0000FFFF;
	}
	return false;
}
