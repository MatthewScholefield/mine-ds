#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "furnace.h"

LitFurnaceUpdater::LitFurnaceUpdater()
{
	loadGraphic(&fireParticle, GRAPHIC_PARTICLE, 0);
	chance = NO_CHANCE;
	for (auto &i : clones)
		setCloneGraphic(&fireParticle, &i);
}

bool LitFurnaceUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	if (bg == false)
	{
		unsigned int databyte = world->data[x][y];
		//Increment time part of data
		unsigned char t = (world->data[x][y] & 0xFF0) >> 4;
		t++;
		if (t == 120)
		{
			//Create new x
			int nx = rand() % 12;
			nx <<= 4 * 3;
			databyte = databyte & 0xFFFF0FFF;
			databyte = databyte | nx;
			t = 0;
		}
		//Replace t
		databyte = databyte & 0xFFFFF00F;
		databyte = databyte | (t << 4);
		//Store in data
		world->data[x][y] = databyte;
		showGraphic(&clones[bufferIndex++], x * 16 - world->camX + ((world->data[x][y]&0xF000) >> 4 * 3), y * 16 - world->camY - ((world->data[x][y]&0xFF0) >> 4) / 32 + 6);
		if (bufferIndex > 15)
			bufferIndex = 0;
		updateSingleBlock(world,x,y,bg);
	}
	return false;
}
