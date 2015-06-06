#include "../blockID.h"
#include "baseMob.h"
#include "hurt.h"
#include "../Config.h"
#include <nds.h>

void collisionWithCactus(WorldObject* world, baseMob* mob, int type, int x, int y, bool sub)
{
	if (mob->timeOnCactus > 40 || mob->timeOnCactus == -1)
	{
		mob->hurt(1, CACTUS_HURT);
		mob->timeOnCactus = 0;
	}
	++mob->timeOnCactus;
	mob->onCactus = true;

}

void cactusCheck(WorldObject* world, baseMob* mob, int type, int x, int y, bool sub)
{
	if (mob->host == true)
	{
		if (world->blocks[x][y] == CACTUS)
			collisionWithCactus(world, mob, type, x, y, sub);
		else if (!(keysHeld() & getGlobalSettings()->getKey(ACTION_CROUCH)) && world->bgblocks[x][y] == CACTUS)
			collisionWithCactus(world, mob, type, x, y, sub);
		else if (mob->timeOnCactus >= 0 && rand() % 5 == 1)
			--mob->timeOnCactus;
	}
}
