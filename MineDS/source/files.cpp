#include <fat.h>
#include <nds.h>
#include <stdio.h>
#include <string>
#include "general.h"
#include "Config.h"
#include "world.h"
#include "inventory.h"
#include "files.h"

void initFile(void)
{
	fatInitDefault();
}

bool saveWorld(worldObject *world)
{
	FILE *fp;

	if ((fp = fopen(WORLD_PATH, "w")) != NULL)
	{
		fwrite(world, sizeof(*world), 1, fp);
		saveInventory(fp);
		fclose(fp);
		return true;
	}
	return false;
}

bool saveControls(Config *controls)
{
	FILE *fp;

	if ((fp = fopen(CONTROLS_PATH, "w")) != NULL)
	{
		fwrite(controls, sizeof(*controls), 1, fp);
		fclose(fp);
		return true;
	}
	return false;
}

bool loadWorld(worldObject *world)
{
	FILE *fp;

	if ((fp = fopen(WORLD_PATH, "r")) != NULL)
	{
		fread(world, sizeof(*world), 1, fp);
		loadInventory(fp);
		fclose(fp);
		return true;
	}
	return false;
}

bool loadControls(Config *controls)
{
	FILE *fp;

	if ((fp = fopen(CONTROLS_PATH, "r")) != NULL)
	{
		fread(controls, sizeof(*controls), 1, fp);
		fclose(fp);
		return true;
	}
	return false;
}
