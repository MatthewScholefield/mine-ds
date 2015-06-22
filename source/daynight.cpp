#include <nds.h>
#include "world.h"
#include "nifi.h"
#include <dswifi9.h>
#include <stdio.h>
#include "general.h"
#include "graphics/graphics.h"

#include "worldRender.h"
int timeTillChange = 0;
u8 r[15] = {12, 16, 15, 14, 13, 12, 11, 8, 8, 7, 6};
u8 g[15] = {15, 22, 22, 18, 16, 13, 12, 8, 7, 5, 4};
u8 b[15] = {31, 31, 30, 29, 28, 27, 26, 26, 24, 23, 22};

u8 r2[15] = {16, 16, 15, 14, 13, 12, 11, 8, 8, 7, 6};
u8 g2[15] = {24, 22, 22, 18, 16, 13, 12, 8, 7, 5, 4};
u8 b2[15] = {31, 31, 30, 29, 28, 27, 26, 26, 24, 23, 22};

void timeUpdate(WorldObject* world)
{
	++timeTillChange;
	if (timeTillChange >= SEC_TO_FPS(5))
	{
		++world->timeInWorld;
		timeTillChange = 0;
		if (world->timeInWorld > 120) world->timeInWorld = 0;
		if (world->timeInWorld >= 80 && world->timeInWorld < 90) world->worldBrightness = world->timeInWorld - 79;
		else if (world->timeInWorld > 110) world->worldBrightness = 10 - (world->timeInWorld - 110);
    else if (world->timeInWorld>=90) world->worldBrightness = 10;
    else world->worldBrightness = 0;
		setSkyColor(r[world->worldBrightness], g[world->worldBrightness], b[world->worldBrightness],
				r2[world->worldBrightness], g2[world->worldBrightness], b2[world->worldBrightness]);
		setSun(world->worldBrightness);
		setBlockPalette(world->worldBrightness);
		if (isWifi())
		{
			unsigned short buffer[100];
			int server_id = getServerID();
			sprintf((char *) buffer, "[DNC: %d %d %d %d %d", server_id, world->worldBrightness, r[world->worldBrightness], g[world->worldBrightness], b[world->worldBrightness]);
			Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
		}
	}
}

bool isDay(WorldObject* world)
{
	return world->timeInWorld >= 0 && world->timeInWorld < 80;
}

