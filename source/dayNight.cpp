#include <nds.h>
#include "WorldObject.h"
#include "nifi.h"
#include <dswifi9.h>
#include <stdio.h>
#include "general.h"
#include "graphics/graphics.h"

#include "worldRender.h"
int timeTillChange = 0;

int lastWorldBrightness = 15;
u8 r[11] = {4, 5, 6, 6, 8, 9, 10, 10, 11, 12, 12};
u8 g[11] = {2, 3, 4, 5, 7, 8, 10, 11, 13, 13, 15};
u8 b[11] = {15, 23, 24, 26, 26, 27, 28, 29, 30, 31, 31};

u8 r2[11] = {6, 7, 8, 8, 11, 12, 13, 14, 15, 16, 16};
u8 g2[11] = {8, 5, 7, 8, 12, 13, 16, 18, 22, 22, 24};
u8 b2[11] = {22, 23, 24, 26, 26, 27, 28, 29, 30, 31, 31};

void dayNightUpdate(WorldObject &world)
{
	++timeTillChange;
	if (timeTillChange >= SEC_TO_FPS(5))
	{
		timeTillChange = 0;
		if (++world.timeInWorld >= 120) world.timeInWorld = 0;
		switch (world.timeInWorld)
		{
		case 0 ... 79:
			world.worldBrightness = 15;
			break;
		case 80 ... 89:
			world.worldBrightness = 15 - (world.timeInWorld - 80);
			break;
		case 90 ... 109:
			world.worldBrightness = 5;
			break;
		case 110 ... 119:
			world.worldBrightness = 5 + (world.timeInWorld - 110);
			break;
		}

		lastWorldBrightness = world.worldBrightness;

		setSkyColor(r[world.worldBrightness - 5], g[world.worldBrightness - 5], b[world.worldBrightness - 5],
					r2[world.worldBrightness - 5], g2[world.worldBrightness - 5], b2[world.worldBrightness - 5]);
		if (isWifi())
		{
			unsigned short buffer[100];
			int server_id = getServerID();
			sprintf((char *) buffer, "[DNC: %d %d %d %d %d", server_id, world.worldBrightness
					, r[world.worldBrightness - 5], g[world.worldBrightness - 5], b[world.worldBrightness - 5]);
			Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
		}
	}
}

bool isDay(WorldObject &world)
{
	return world.timeInWorld >= 0 && world.timeInWorld < 80;
}

void resetSky()
{
	setSkyColor(r[lastWorldBrightness - 5], g[lastWorldBrightness - 5], b[lastWorldBrightness - 5]
				, r2[lastWorldBrightness - 5], g2[lastWorldBrightness - 5], b2[lastWorldBrightness - 5]);
}

