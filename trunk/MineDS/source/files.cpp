#include <fat.h>
#include <nds.h>
#include <stdio.h>
#include <string>
#include "general.h"
#include "Config.h"
#include "world.h"
#include "inventory.h"
#include "files.h"
#include "mobs/mobHandler.h"

void initFile(void)
{
	fatInitDefault();
}

bool saveWorld(WorldObject *world)
{
	FILE *fp;

	if ((fp = fopen(WORLD_PATH, "wb+")) != NULL)
	{
		fwrite(world, sizeof (*world), 1, fp);
		saveInventory(fp);
		savePlayer(fp);
		fclose(fp);
		return true;
	}
	return false;
}

bool saveControls(Config *controls)
{
	FILE *fp;

	if ((fp = fopen(CONTROLS_PATH, "wb+")) != NULL)
	{
		fwrite(controls, sizeof (*controls), 1, fp);
		fclose(fp);
		return true;
	}
	return false;
}

bool loadWorld(WorldObject *world)
{
	FILE *fp;

	if ((fp = fopen(WORLD_PATH, "rb")) != NULL)
	{
		fread(world, sizeof (*world), 1, fp);
		loadInventory(fp);
		loadPlayer(fp);
		fclose(fp);
		return true;
	}
	return false;
}

bool loadControls(Config *controls)
{
	FILE *fp;

	if ((fp = fopen(CONTROLS_PATH, "rb")) != NULL)
	{
		fread(controls, sizeof (*controls), 1, fp);
		fclose(fp);
		return true;
	}
	return false;
}
