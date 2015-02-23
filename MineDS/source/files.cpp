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

bool saveWorld(worldObject *world)
{
	FILE *worldFile;
	FILE *inventoryFile;
	FILE *mobsFile;

	if ((worldFile = fopen(WORLD_PATH, "wb+")) != NULL &&
			(inventoryFile = fopen(INVENTORY_PATH, "w+")) != NULL &&
			(mobsFile = fopen(MOBS_PATH, "w+")) != NULL)
	{
		fwrite(world, sizeof (*world), 1, worldFile);
		fclose(worldFile);

		saveMobs(mobsFile);
		fclose(mobsFile);

		saveInventory(inventoryFile);
		fclose(inventoryFile);
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

bool loadWorld(worldObject *world)
{
	FILE *worldFile;
	FILE *inventoryFile;
	FILE *mobsFile;

	if ((worldFile = fopen(WORLD_PATH, "rb")) != NULL &&
			(inventoryFile = fopen(INVENTORY_PATH, "r")) != NULL
		&& (mobsFile = fopen(MOBS_PATH, "r")) != NULL)
	{
		fread(world, sizeof (*world), 1, worldFile);
		fclose(worldFile);

		loadInventory(inventoryFile);
		fclose(inventoryFile);

		loadMobs(mobsFile);
		fclose(mobsFile);
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
