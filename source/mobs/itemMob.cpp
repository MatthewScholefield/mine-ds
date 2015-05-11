#include <stdio.h>
#include <math.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "itemMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../blocks.h"
#include "../general.h"
#include "../sounds.h"
#include "../collision.h"
#include "../inventory.h"
#include "mobFunctions.h"
#include "../mining.h"

Graphic *itemGraphics[NUM_BLOCKS];
bool inUse[NUM_BLOCKS] = {false};

void itemGraphicUpdate()
{
	for (int i = 0; i < NUM_BLOCKS; ++i)
	{
		if (!inUse[i] && itemGraphics[i])
		{
			unloadGraphic(itemGraphics[i]);
			delete itemGraphics[i];
			itemGraphics[i] = NULL;
		}
		inUse[i] = false;
	}
}

itemMob::itemMob(int a, int b)
{
	sx = 8;
	sy = 8;
	x = a;
	y = b;
	health = 1;
	mobtype = 8;
	blockID = -1;
	displayID = -1;
	amount = -1;
	floatY = 0;
	//Set initial velocity
	vx = double((rand() % 10) + 20) / 100.0;
	//Set direction
	vx *= (rand() % 2) ? -1 : 1;
}

bool itemMob::isMyPlayer()
{
	return false;
}

void itemMob::updateMob(WorldObject* world)
{
	if (!alive)
		return;
	if (!itemGraphics[blockID])
	{
		itemGraphics[blockID] = new Graphic();
		loadGraphicMiniBlock(itemGraphics[blockID], displayID, 8, 8);
	}
	inUse[blockID] = true;
	if (vx != 0)//(vx > 0&& isBlockWalkThrough(world->blocks[int ((x + sx) / 16)][y / 16])) || (vx < 0 && isBlockWalkThrough(world->blocks[int(x / 16)][y / 16])))
	{
		bool positive = vx > 0;
		vx -= positive ? 0.005 : -0.005;
		if ((positive && vx < 0) || (!positive && vx > 0))
			vx = 0;
	}
	else if (vx != 0)
		vx = 0;
	++floatY;
	if (floatY > 100)
		floatY = 0;
	baseMob* target;
	target = mobHandlerFindMob(8, 2, x, y - 8);
	if (target == NULL)
		target = mobHandlerFindMob(8, 2, x, y - 24);
	if (target == NULL || !target->isMyPlayer())
		showGraphic(itemGraphics[blockID], x - world->camX - 3, (y - 8 - world->camY + int((4.0 * sin(double(floatY)*6.28 / 100.0)))), false);
	else
	{
		addInventory(blockID, amount);
		alive = false;
		killMob();
	}
	if (!onScreen(x, y, world->camX, world->camY) && rand() % 1000 == 1)
		killMob();
}

void itemMob::sendWifiUpdate()
{
}

void itemMob::saveToFile(FILE* pFile)
{
}

void itemMob::killMob()
{
	timeTillWifiUpdate = 1;
	alive = false;
}

void itemMob::loadFromFile(FILE* pFile)
{
	killMob();
}

void itemMob::hurt(int hamount, int type)
{
  if (type == PROPERTY_HURT)
  {
	  if (blockID == -1)
	  	displayID = blockID = hamount;
	  else if (amount == - 1)
	  	amount = hamount;
	  else if (hamount > 0)
	  	displayID = hamount;
  }
  else killMob();
}
