#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "air.h"

AirUpdater::AirUpdater()
{
	blockID = AIR;
	chance = NO_CHANCE;
}

void AirUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	if (bg == false)
	{
		//Remove the foregraound data (x&0 = 0)
		world->data[x][y] &= 0xFFFF0000;
	}
	else
	{
		//Remove the background data (x&0 = 0)
		world->data[x][y] &= 0x0000FFFF;
	}
}
