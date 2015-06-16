#include "../world.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "../blockupdate.h"
#include <stdlib.h>
#include "../blockID.h"
#include "furnace.h"

furnaceUpdater::furnaceUpdater()
{
	loadGraphic(&fireParticle, GRAPHIC_PARTICLE, 0);
	chance = NO_CHANCE;
	blockID = FURNACE_LIT;
}

void furnaceUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	if (bg == false)
	{
		int databyte = world->data[x][y];
		//Increment time part of data
		char t = world->data[x][y] & 0xFF;
		t++;
		if (t == 120)
		{
			//Create new x
			int nx = rand() % 12;
			nx *= 256;
			databyte = databyte & 0x0FF;
			databyte = databyte | nx;
			t = 0;
		}
		//Replace t
		databyte = databyte & 0xF00;
		databyte = databyte | t;
		//Store in data
		world->data[x][y] = databyte;
		showGraphic(&fireParticle, x * 16 - world->camX + (world->data[x][y]&0xF00) / 256, y * 16 - world->camY - (world->data[x][y]&0xFF) / 32 + 6);

	}
}