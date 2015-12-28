#include "../world/WorldObject.h"
#include "../general.h"
#include "../graphics/graphics.h"
#include "blockUpdater.h"
#include <stdlib.h>
#include "../blockID.h"
#include "LitFurnaceUpdater.h"
#include "../furnaceHandler.h"

Graphic LitFurnaceUpdater::fireParticle(GraphicType::PARTICLE, 0);

LitFurnaceUpdater::LitFurnaceUpdater()
{
	chance = NO_CHANCE;
}

bool LitFurnaceUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	if (bg == false)
	{
		unsigned int databyte = world.data[x][y];
		//Increment time part of data
		unsigned char t = (world.data[x][y] & 0xFF0) >> 4;
		t++;
		if (t == 120)
		{
			//Create new x
			int nx = rand() % 12;
			nx <<= 4 * 3;
			databyte = databyte & 0xFFFF0FFF;
			databyte = databyte | nx;
			t = 0;
			addParticle(Particle((float) (x * 16 + ((world.data[x][y]&0xF000) >> 4 * 3))
								, (float) y * 16 + 10, (float) (rand() % 4 - 1) / 32
								, -0.05f, 0.0f, -0.008f, 120, fireParticle));
		}
		//Replace t
		databyte = databyte & 0xFFFFF00F;
		databyte = databyte | (t << 4);
		//Store in data
		world.data[x][y] = databyte;
	}
	int id = getFurnaceID(world, x, y, bg);
	if (--world.furnaces[id].timeTillFuelBurn < 0)
	{
		world.furnaces[id].timeTillFuelBurn = SEC_TO_FPS(4);
		if (--world.furnaces[id].fuelTillComplete < 0)
		{
			createResult(world.furnaces[id]);
			world.furnaces[id].fuelTillComplete = fuelNeeded(world.furnaces[id]);
		}
		if (--world.furnaces[id].fuel < 0)
		{
			if (fuelNeeded(world.furnaces[id]) > 0)
				convertItemToFuel(world.furnaces[id]);
			if (world.furnaces[id].fuel < 0)
			{
				if (bg)
					world.bgblocks[x][y] = FURNACE;
				else
					world.blocks[x][y] = FURNACE;
			}
		}
	}
	updateSingleBlock(world, x, y, bg);
	return false;
}
