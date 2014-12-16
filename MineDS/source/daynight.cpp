#include <nds.h>
#include "world.h"
#include "nifi.h"
#include <dswifi9.h>
#include <stdio.h>
#include <string.h>

#include "worldRender.h"
int timeTillChange = 0;
int r[15] = {16, 16, 15, 14, 13, 12, 11, 8, 8, 7, 6};
int g[15] = {24, 22, 22, 18, 16, 13, 12, 8, 7, 5, 4};
int b[15] = {31, 31, 30, 29, 28, 27, 26, 26, 24, 23, 22};

void timeUpdate(worldObject* world)
{
	timeTillChange++;
	if (timeTillChange >= 300)
	{
		world->timeInWorld++;
		timeTillChange = 0;
		if (world->timeInWorld > 120) world->timeInWorld = 0;
		if (world->timeInWorld >= 80 && world->timeInWorld < 90) world->worldBrightness++;
		else if (world->timeInWorld > 110) world->worldBrightness--;
		setBackdropColor(RGB15(r[world->worldBrightness], g[world->worldBrightness], b[world->worldBrightness]));
		setSun(world->worldBrightness);
		if (isWifi())
		{
			unsigned short buffer[100];
			int server_id = getServerID();
			sprintf((char *) buffer, "[DNC: %d %d %d %d %d", server_id, world->worldBrightness, r[world->worldBrightness], g[world->worldBrightness], b[world->worldBrightness]);
			Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
		}
	}
}

