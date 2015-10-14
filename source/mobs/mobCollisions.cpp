#include "../blockID.h"
#include "BaseMob.h"
#include "hurt.h"
#include "../Config.h"
#include <nds.h>

static void collisionWithCactus(BaseMob *mob)
{
	if (mob->timeOnCactus > 40 || mob->timeOnCactus == -1)
	{
		mob->hurt(1, CACTUS_HURT);
		mob->timeOnCactus = 0;
	}
	++mob->timeOnCactus;
	mob->onCactus = true;

}

void cactusCheck(WorldObject &world, BaseMob *mob, int x, int y)
{
	if (mob->host == true)
	{
		if (world.blocks[x][y] == CACTUS)
			collisionWithCactus(mob);
		else if (!(keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH)) && world.bgblocks[x][y] == CACTUS)
			collisionWithCactus(mob);
		else if (mob->timeOnCactus >= 0 && rand() % 5 == 1)
			--mob->timeOnCactus;
	}
}
