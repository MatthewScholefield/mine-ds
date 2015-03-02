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
	
	bool openedWorld = (worldFile = fopen(WORLD_PATH, "w+")) != NULL;
	bool openedInventory = (inventoryFile = fopen(INVENTORY_PATH, "w+")) != NULL;
	bool openedMobs = (mobsFile = fopen(MOBS_PATH, "w+")) != NULL;

	if (openedWorld && openedInventory && openedMobs)
	{
		//fwrite(world, sizeof (*world), 1, worldFile);
		fprintf(worldFile, "%d ", world->gamemode);
		for (int i = 0; i <= WORLD_WIDTH; ++i)
		{
			for (int j = 0; j <= WORLD_HEIGHT; ++j)
				fprintf(worldFile, "%d %d %d ", world->blocks[i][j], world->bgblocks[i][j], world->data[i][j]);
			if (i % 50 == 0)
				iprintf("\x1b[19;1HSaving... %d%%", int(100 * (double(i) / double(WORLD_WIDTH))));
		}
		iprintf("\x1b[19;1H              ");
		fclose(worldFile);

		saveMobs(mobsFile);
		fclose(mobsFile);

		saveInventory(inventoryFile);
		fclose(inventoryFile);
		return true;
	}
	if (openedWorld)
		fclose(worldFile);
	if (openedInventory)
		fclose(inventoryFile);
	if (openedMobs)
		fclose(mobsFile);
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
	
	bool openedWorld = (worldFile = fopen(WORLD_PATH, "r")) != NULL;
	bool openedInventory = (inventoryFile = fopen(INVENTORY_PATH, "r")) != NULL;
	bool openedMobs = (mobsFile = fopen(MOBS_PATH, "r")) != NULL;
	
	if (openedWorld && openedInventory && openedMobs)
	{
		int loadGameMode;
		fscanf(worldFile, "%d ", &loadGameMode);
		world->gamemode = gamemode_t(loadGameMode);
		for (int i = 0; i <= WORLD_WIDTH; ++i)
		{
			for (int j = 0; j <= WORLD_HEIGHT; ++j)
				fscanf(worldFile, "%d %d %d ", &world->blocks[i][j], &world->bgblocks[i][j], &world->data[i][j]);
			if (i % 50 == 0)
				iprintf("\x1b[22;1HLoading... %d%%", int(100 * (double(i) / double(WORLD_WIDTH))));

		}
		iprintf("\x1b[22;1H              ");
		fclose(worldFile);

		loadInventory(inventoryFile);
		fclose(inventoryFile);

		loadMobs(mobsFile);
		fclose(mobsFile);
		return true;
	}
	if (openedWorld)
		fclose(worldFile);
	if (openedInventory)
		fclose(inventoryFile);
	if (openedMobs)
		fclose(mobsFile);
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
