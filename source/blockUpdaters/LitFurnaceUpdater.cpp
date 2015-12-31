#include "../world/World.h"
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

bool LitFurnaceUpdater::update(World &world, int x, int y, bool bg)
{
	int id = getFurnaceID(world, x, y, bg);
	if (bg == false)
	{
		unsigned int databyte = world.data[x][y];
		//Increment time part of data
		unsigned char t = (world.data[x][y] & 0xFF00) >> 4 * 2;
		++t;
		if (t == 120)
		{
			//Create new x
			world.furnaces[id].particleX = rand() % 12;
			t = 0;
			addParticle(Particle((float) (x * 16 + world.furnaces[id].particleX)
								, (float) y * 16 + 10, (float) (rand() % 3 - 1) / 32
								, -0.05f, 0.0f, -0.008f, 120, fireParticle));
		}
		//Replace t
		databyte = databyte & 0xFFFF00FF;
		databyte = databyte | (t << 4 * 2);
		//Store in data
		world.data[x][y] = databyte;
	}
	if (--world.furnaces[id].timeTillFuelBurn < 0)
	{
		world.furnaces[id].timeTillFuelBurn = SEC_TO_FPS(4);
		if (--world.furnaces[id].fuelTillComplete < 0)
		{
			createResult(world.furnaces[id]);
			world.furnaces[id].fuelTillComplete = fuelNeeded(world.furnaces[id]);
		}
		if (--world.furnaces[id].fuelAmount < 0)
		{
			if (fuelNeeded(world.furnaces[id]) > 0)
				convertItemToFuel(world.furnaces[id]);
			if (world.furnaces[id].fuelAmount < 0)
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
