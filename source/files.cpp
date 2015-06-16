#include <fat.h>
#include <nds.h>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include "general.h"
#include "Config.h"
#include "world.h"
#include "inventory.h"
#include "files.h"
#include "mobs/mobHandler.h"
#include "worldRender.h"
#include "graphics/UI.h"
#include "graphics/Button.h"
#include "graphics/graphics.h"

void initFile(void)
{
	fatInitDefault();
	mkdir(MINE_DS_FOLDER, 0777);
}

bool saveWorld(WorldObject *world)
{
	FILE *worldFile;

	bool openedWorld = (worldFile = fopen(MINE_DS_FOLDER WORLD_FILENAME, "w+")) != NULL;

	if (openedWorld)
	{
		fprintf(worldFile, VERSION_STRING);
		fprintf(worldFile, " %d %d ", WORLD_WIDTH, WORLD_HEIGHT);
		fprintf(worldFile, "%d ", world->gamemode);
		fprintf(worldFile, "%d ", world->timeInWorld);
		for (int i = 0; i <= WORLD_WIDTH; ++i)
		{
			for (int j = 0; j <= WORLD_HEIGHT; ++j)
				fprintf(worldFile, "%d %d %d ", world->blocks[i][j], world->bgblocks[i][j], world->data[i][j]);
			if (i % 50 == 0)
				iprintf("\x1b[19;1HSaving... %d%%", int(100 * (double(i) / double(WORLD_WIDTH))));
		}
		for (int i = 0; i <= WORLD_WIDTH; ++i)
			fprintf(worldFile, "%d ", world->biome[i]);

		saveInventory(worldFile);
		saveMobs(worldFile);
		for (int i = 0; i < MAX_CHESTS; ++i)
			for (int j = 0; j < CHEST_SLOTS; ++j)
				fprintf(worldFile, "%d %d ", world->chests[i][j][INDEX_BLOCK_ID], world->chests[i][j][INDEX_AMOUNT]);
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

	if ((fp = fopen(MINE_DS_FOLDER CONTROLS_FILENAME, "w+")) != NULL)
	{
		fprintf(fp, "==Controls==\n");
		fprintf(fp, "Move Left: %s\n", getKeyChar(controls->getKey(ACTION_MOVE_LEFT)));
		fprintf(fp, "Move Right: %s\n", getKeyChar(controls->getKey(ACTION_MOVE_RIGHT)));
		fprintf(fp, "Jump: %s\n", getKeyChar(controls->getKey(ACTION_JUMP)));
		fprintf(fp, "Crouch: %s\n", getKeyChar(controls->getKey(ACTION_CROUCH)));
		fprintf(fp, "Item Left: %s\n", getKeyChar(controls->getKey(ACTION_ITEM_LEFT)));
		fprintf(fp, "Item Right: %s\n", getKeyChar(controls->getKey(ACTION_ITEM_RIGHT)));
		fprintf(fp, "Switch Screen: %s\n", getKeyChar(controls->getKey(ACTION_SWITCH_SCREEN)));
		fprintf(fp, "Menu: %s\n", getKeyChar(controls->getKey(ACTION_MENU)));
		fprintf(fp, "Climb: %s\n", getKeyChar(controls->getKey(ACTION_CLIMB)));
		fprintf(fp, "Drop: %s\n", getKeyChar(controls->getKey(ACTION_DROP)));
		fprintf(fp, "\n==Game Options==\n");
		fprintf(fp, "Herobrine: %s\n", controls->getProperty(PROPERTY_HEROBRINE) ? "Enabled" : "Disabled");
		fprintf(fp, "Draw Mode: %s\n", controls->getProperty(PROPERTY_DRAW) ? "Enabled" : "Disabled");
		fprintf(fp, "Smooth Camera: %s\n", controls->getProperty(PROPERTY_SMOOTH) ? "Enabled" : "Disabled");
		fprintf(fp, "Creative Speed: %s\n", controls->getProperty(PROPERTY_SPEED) ? "Enabled" : "Disabled");
		fprintf(fp, "\nTexture Pack: %s\n", controls->textureName.c_str());
		fclose(fp);
		return true;
	}
	return false;
}

bool loadWorld(WorldObject *world)
{
	FILE *worldFile;

	bool openedWorld = (worldFile = fopen(MINE_DS_FOLDER WORLD_FILENAME, "r")) != NULL;

	if (openedWorld)
	{
		delete world;
		world = new WorldObject();
		char *versionChar = new char();
		fscanf(worldFile, "%s ", versionChar);
		int worldBlocksX, worldBlocksY;
		fscanf(worldFile, "%d %d ", &worldBlocksX, &worldBlocksY);
		if (worldBlocksX != WORLD_WIDTH || worldBlocksY != WORLD_HEIGHT || strcmp(versionChar, VERSION_STRING) != 0)
		{
			consoleClear();
			drawBackground();
			if (strcmp(versionChar, VERSION_STRING) != 0)
				printXY(4, 9, "Incorrect World Version");
			else
				printXY(5, 9, "Incorrect Block Count");
			Button attempt(8, 11, "Attempt Load");
			Button abort(8, 16, "Abort", 14);
			Button buttons[] = {attempt, abort};
			switch (menu(buttons, 2, false))
			{
				case 1:
					consoleClear();
					drawBackground();
					break;
				case 2:
					fclose(worldFile);
					delete versionChar;
					return false;
			}
		}
		delete versionChar;
		int loadGameMode;
		fscanf(worldFile, "%d ", &loadGameMode);
		world->gamemode = gamemode_t(loadGameMode);
		int loadTimeInWorld;
		fscanf(worldFile, "%d ", &loadTimeInWorld);
		world->timeInWorld = loadTimeInWorld;
		for (int i = 0; i <= worldBlocksX; ++i)
		{
			for (int j = 0; j <= worldBlocksY; ++j)
				fscanf(worldFile, "%d %d %d ", &world->blocks[i][j], &world->bgblocks[i][j], &world->data[i][j]);
			if (i % 64 == 0)
				iprintf("\x1b[22;1HLoading... %d%%", int(100 * (double(i) / double(WORLD_WIDTH))));

		}
		Calculate_Brightness(world);
		int loadBiome;
		for (int i = 0; i <= WORLD_WIDTH; ++i)
		{
			fscanf(worldFile, "%d ", &loadBiome);
			world->biome[i] = Biome(loadBiome);
		}
		loadInventory(worldFile);
		loadMobs(worldFile);
		for (int i = 0; i < MAX_CHESTS; ++i)
			for (int j = 0; j < CHEST_SLOTS; ++j)
				fscanf(worldFile, "%d %d ", &world->chests[i][j][INDEX_BLOCK_ID], &world->chests[i][j][INDEX_AMOUNT]);
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

	if ((fp = fopen(MINE_DS_FOLDER CONTROLS_FILENAME, "r")) != NULL)
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
		fscanf(fp, "Item Left: %s\n", &parseChar);
		controls->setKey(ACTION_ITEM_LEFT, parseKeyChar(&parseChar));
		fscanf(fp, "Item Right: %s\n", &parseChar);
		controls->setKey(ACTION_ITEM_RIGHT, parseKeyChar(&parseChar));
		fscanf(fp, "Switch Screen: %s\n", &parseChar);
		controls->setKey(ACTION_SWITCH_SCREEN, parseKeyChar(&parseChar));
		fscanf(fp, "Menu: %s\n", &parseChar);
		controls->setKey(ACTION_MENU, parseKeyChar(&parseChar));
		fscanf(fp, "Climb: %s\n", &parseChar);
		controls->setKey(ACTION_CLIMB, parseKeyChar(&parseChar));
		fscanf(fp, "Drop: %s\n", &parseChar);
		controls->setKey(ACTION_DROP, parseKeyChar(&parseChar));
		fscanf(fp, "\n==Game Options==\n");
		fscanf(fp, "Herobrine: %s\n", &parseChar);
		controls->setProperty(PROPERTY_HEROBRINE, parsePropertyChar(&parseChar));
		fscanf(fp, "Draw Mode: %s\n", &parseChar);
		controls->setProperty(PROPERTY_DRAW, parsePropertyChar(&parseChar));
		fscanf(fp, "Smooth Camera: %s\n", &parseChar);
		controls->setProperty(PROPERTY_SMOOTH, parsePropertyChar(&parseChar));
		fscanf(fp, "Creative Speed: %s\n", &parseChar);
		controls->setProperty(PROPERTY_SPEED, parsePropertyChar(&parseChar));
		fscanf(fp, "\nTexture Pack: %s\n", &parseChar);
		controls->textureName = &parseChar;
		fclose(fp);
		return true;
	}
	return false;
}

bool loadTexture(const char *fileName)
{
	std::string temp(fileName);
	temp = MINE_DS_FOLDER TEXTURE_FOLDER + temp;
	FILE *texFile = fopen(temp.c_str(), "rb");
	if (!texFile)
	{
		loadDefaultTexture();
		updateTexture();
		if (strcmp(fileName, "default") == 0)
			return true;
		return false;
	}

	unsigned int *blockTilesMem = new unsigned int[TILES_ARRAY_LEN];
	unsigned short *blockPalMem = new unsigned short[PAL_ARRAY_LEN];
	if (fread(blockTilesMem, sizeof (uint32_t), TILES_ARRAY_LEN, texFile) != TILES_ARRAY_LEN || fread(blockPalMem, sizeof (unsigned short), PAL_ARRAY_LEN, texFile) != PAL_ARRAY_LEN)
	{
		delete[] blockTilesMem;
		delete[] blockPalMem;
		blockTilesMem = NULL;
		blockPalMem = NULL;
	}

	unsigned int *mobTilesMem = new unsigned int[TILES_ARRAY_LEN];
	unsigned short *mobPalMem = new unsigned short[PAL_ARRAY_LEN];
	if (fread(mobTilesMem, sizeof (uint32_t), TILES_ARRAY_LEN, texFile) != TILES_ARRAY_LEN || fread(mobPalMem, sizeof (unsigned short), PAL_ARRAY_LEN, texFile) != PAL_ARRAY_LEN)
	{
		delete[] mobTilesMem;
		delete[] mobPalMem;
		mobTilesMem = NULL;
		mobPalMem = NULL;
	}

	unsigned int *subBgTilesMem = new unsigned int[TILES_ARRAY_LEN];
	unsigned short *subBgPalMem = new unsigned short[PAL_ARRAY_LEN];
	if (fread(subBgTilesMem, sizeof (uint32_t), TILES_ARRAY_LEN, texFile) != TILES_ARRAY_LEN || fread(subBgPalMem, sizeof (unsigned short), PAL_ARRAY_LEN, texFile) != PAL_ARRAY_LEN)
	{
		delete[] subBgTilesMem;
		delete[] subBgPalMem;
		subBgTilesMem = NULL;
		subBgPalMem = NULL;
	}

	fclose(texFile);
	loadTexture(blockTilesMem, blockPalMem, mobTilesMem, mobPalMem, subBgTilesMem, subBgPalMem);
	updateTexture();

	delete[] blockTilesMem;
	delete[] blockPalMem;
	delete[] mobTilesMem;
	delete[] mobPalMem;
	delete[] subBgTilesMem;
	delete[] subBgPalMem;
	return true;
}