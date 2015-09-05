#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "furnace.h"

FurnaceUpdater::FurnaceUpdater()
{
	loadGraphic(&fireParticle, GRAPHIC_PARTICLE, 0);
	chance = NO_CHANCE;
	for (auto &i : clones)
		setCloneGraphic(&fireParticle, &i);
}

bool FurnaceUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	if (bg == false)
	{
		unsigned int databyte = world->data[x][y];
		//Increment time part of data
		unsigned char t = world->data[x][y] & 0xFF;
		t++;
		if (t == 120)
		{
			//Create new x
			int nx = rand() % 12;
			nx <<= 8;
			databyte = databyte & 0x0FF;
			databyte = databyte | nx;
			t = 0;
		}
		//Replace t
		databyte = databyte & 0xF00;
		databyte = databyte | t;
		//Store in data
		world->data[x][y] = databyte;
		showGraphic(&clones[bufferIndex++], x * 16 - world->camX + ((world->data[x][y]&0xF00) >> 8), y * 16 - world->camY - (world->data[x][y]&0xFF) / 32 + 6);
		if (bufferIndex > 15)
			bufferIndex = 0;
		updateSingleBlock(world,x,y,bg);
	}
	return false;
}
