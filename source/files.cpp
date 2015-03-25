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

	bool openedWorld = (worldFile = fopen(WORLD_PATH, "w+")) != NULL;

	if (openedWorld)
	{
		//fwrite(world, sizeof (*world), 1, worldFile);
		fprintf(worldFile, "%d ", world->gamemode);
		for (int i = 0; i <= WORLD_WIDTH; ++i)
		{
			for (int j = 0; j <= WORLD_HEIGHT; ++j)
				fprintf(worldFile, "%d %d %d ", world->blocks[i][j], world->bgblocks[i][j], world->data[i][j]);
			if (i % 50 == 0)
				iprintf("\x1b[19;1HSaving... %d%%", int(100 * (double(i) / double(WORLD_WIDTH + 100))));
		}
		for (int i = 0; i <= WORLD_WIDTH; ++i)
			fprintf(worldFile, "%d ", world->biome[i]);

		saveInventory(worldFile);
		saveMobs(worldFile);
		fclose(worldFile);
		iprintf("\x1b[19;1H              ");
		return true;
	}
	if (openedWorld)
		fclose(worldFile);
	return false;
}

bool saveControls(Config *controls)
{
	FILE *fp;

	if ((fp = fopen(CONTROLS_PATH, "w+")) != NULL)
	{
		fprintf(fp, "==Controls==\n");
		fprintf(fp, "Move Left: %s\n", getKeyChar(controls->getKey(ACTION_MOVE_LEFT)));
		fprintf(fp, "Move Right: %s\n", getKeyChar(controls->getKey(ACTION_MOVE_RIGHT)));
		fprintf(fp, "Jump: %s\n", getKeyChar(controls->getKey(ACTION_JUMP)));
		fprintf(fp, "Crouch: %s\n", getKeyChar(controls->getKey(ACTION_CROUCH)));
		fprintf(fp, "ItemLeft: %s\n", getKeyChar(controls->getKey(ACTION_ITEM_LEFT)));
		fprintf(fp, "Item Right: %s\n", getKeyChar(controls->getKey(ACTION_ITEM_RIGHT)));
		fprintf(fp, "Switch Screen: %s\n", getKeyChar(controls->getKey(ACTION_SWITCH_SCREEN)));
		fprintf(fp, "Menu: %s\n", getKeyChar(controls->getKey(ACTION_MENU)));
		fprintf(fp, "Climb: %s\n", getKeyChar(controls->getKey(ACTION_CLIMB)));
		fprintf(fp, "\n==Game Options");
		fprintf(fp, "Herobrine: %s", controls->getProperty(PROPERTY_HEROBRINE) ? "Enabled" : "Disabled");
		fprintf(fp, "Draw Mode: %s", controls->getProperty(PROPERTY_DRAW) ? "Enabled" : "Disabled");
		fprintf(fp, "Smooth Camera: %s", controls->getProperty(PROPERTY_SMOOTH) ? "Enabled" : "Disabled");
		fprintf(fp, "Creative Speed: %s", controls->getProperty(PROPERTY_SPEED) ? "Enabled" : "Disabled");
		fclose(fp);
		return true;
	}
	return false;
}

bool loadWorld(worldObject *world)
{
	FILE *worldFile;

	bool openedWorld = (worldFile = fopen(WORLD_PATH, "r")) != NULL;

	if (openedWorld)
	{
		int loadGameMode;
		fscanf(worldFile, "%d ", &loadGameMode);
		world->gamemode = gamemode_t(loadGameMode);
		for (int i = 0; i <= WORLD_WIDTH; ++i)
		{
			for (int j = 0; j <= WORLD_HEIGHT; ++j)
				fscanf(worldFile, "%d %d %d ", &world->blocks[i][j], &world->bgblocks[i][j], &world->data[i][j]);
			if (i % 64 == 0)
				iprintf("\x1b[22;1HLoading... %d%%", int(100 * (double(i) / double(WORLD_WIDTH + 100))));
		}
		int loadBiome;
		for (int i = 0; i <= WORLD_WIDTH; ++i)
		{
			fscanf(worldFile, "%d ", &loadBiome);
			world->biome[i] = Biome(loadBiome);
		}
		loadInventory(worldFile);
		loadMobs(worldFile);
		iprintf("\x1b[22;1H              ");
		fclose(worldFile);
		return true;
	}
	if (openedWorld)
		fclose(worldFile);
	return false;
}

bool loadControls(Config *controls)
{
	FILE *fp;

	if ((fp = fopen(CONTROLS_PATH, "r")) != NULL)
	{
		char parseChar;
		fscanf(fp, "==Controls==\n");
		fscanf(fp, "Move Left: %s\n", &parseChar);
		controls->setKey(ACTION_MOVE_LEFT, parseKeyChar(&parseChar));
		fscanf(fp, "Move Right: %s\n", &parseChar);
		controls->setKey(ACTION_MOVE_RIGHT, parseKeyChar(&parseChar));
		fscanf(fp, "Jump: %s\n", &parseChar);
		controls->setKey(ACTION_JUMP, parseKeyChar(&parseChar));
		fscanf(fp, "Crouch: %s\n", &parseChar);
		controls->setKey(ACTION_CROUCH, parseKeyChar(&parseChar));
		fscanf(fp, "ItemLeft: %s\n", &parseChar);
		controls->setKey(ACTION_ITEM_LEFT, parseKeyChar(&parseChar));
		fscanf(fp, "Item Right: %s\n", &parseChar);
		controls->setKey(ACTION_ITEM_RIGHT, parseKeyChar(&parseChar));
		fscanf(fp, "Switch Screen: %s\n", &parseChar);
		controls->setKey(ACTION_SWITCH_SCREEN, parseKeyChar(&parseChar));
		fscanf(fp, "Menu: %s\n", &parseChar);
		controls->setKey(ACTION_MENU, parseKeyChar(&parseChar));
		fscanf(fp, "Climb: %s\n", &parseChar);
		controls->setKey(ACTION_CLIMB, parseKeyChar(&parseChar));
		fprintf(fp, "\n==Game Options");
		fprintf(fp, "Herobrine: %s", &parseChar);
		controls->setProperty(PROPERTY_HEROBRINE, parsePropertyChar(&parseChar));
		fprintf(fp, "Draw Mode: %s", &parseChar);
		controls->setProperty(PROPERTY_DRAW, parsePropertyChar(&parseChar));
		fprintf(fp, "Smooth Camera: %s", &parseChar);
		controls->setProperty(PROPERTY_SMOOTH, parsePropertyChar(&parseChar));
		fprintf(fp, "Creative Speed: %s", &parseChar);
		controls->setProperty(PROPERTY_SPEED, parsePropertyChar(&parseChar));
		fclose(fp);
		return true;
	}
	return false;
}
