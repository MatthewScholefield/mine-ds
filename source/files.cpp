#include <fat.h>
#include <nds.h>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <filesystem.h>
#include <unistd.h>
#include "general.h"
#include "Config.h"
#include "world/World.h"
#include "inventory.h"
#include "files.h"
#include "mobs/mobHandler.h"
#include "world/worldRender.h"
#include "graphics/UI.h"
#include "graphics/Button.h"
#include "graphics/graphics.h"
#include "sounds.h"
#include "chests.h" //For stopping sound on save / load
#include "furnaceHandler.h"
#include "blocks.h"

void initFile()
{
#ifdef USE_NITRO
	nitroFSInit(nullptr);
	chdir("nitro:/");
#endif
#ifdef USE_FAT
	fatInitDefault();
	chdir("fat:/");
	mkdir("data",0777);
	chdir("fat:/data");
	mkdir("Mine-DS", 0777);
	chdir("fat:/data/Mine-DS");
	mkdir("Textures", 0777);
	chdir("fat:/");
#endif
}

bool saveWorld(World &world)
{
	if (!SHOULD_SAVE) return false;
	stopMusic();
	FILE *worldFile;
	bool openedWorld = (worldFile = fopen(MINE_DS_FOLDER WORLD_FILENAME, "w")) != nullptr;

	if (openedWorld)
	{
		fprintf(worldFile, VERSION_STRING " ");
		fprintf(worldFile, "%d %d ", World::WIDTH, World::HEIGHT);
		fprintf(worldFile, "%d ", (int)world.gameMode);
		fprintf(worldFile, "%d ", world.timeInWorld);
		for (int i = 0; i < World::WIDTH; ++i)
		{
			for (int j = 0; j < World::HEIGHT; ++j)
				fprintf(worldFile, "%hd %hd %d ", world.blocks[i][j], world.bgblocks[i][j], world.data[i][j]);
			if (i % 50 == 0)
				printf("\x1b[19;1HSaving... %d%%", int(100 * (double(i) / double(World::WIDTH))));
		}
		for (int i = 0; i < World::WIDTH; ++i)
			fprintf(worldFile, "%d ", (int)world.biome[i]);

		saveInventory(worldFile);
		saveMobs(worldFile);
		saveChests(worldFile, world);
		saveFurnaces(worldFile, world);
		fprintf(worldFile, "%d ", world.reservedWater);
		fclose(worldFile);
		printf("\x1b[19;1H              ");
		fclose(worldFile);
		playMusic(MUSIC_CALM);
		return true;
	}
	if (openedWorld)
		fclose(worldFile);
	playMusic(MUSIC_CALM);
	return false;
}

bool loadWorld(World *world)
{
	if (!SHOULD_LOAD) return false;
	stopMusic();
	FILE *worldFile;

	bool openedWorld = (worldFile = fopen(MINE_DS_FOLDER WORLD_FILENAME, "r")) != nullptr;

	if (openedWorld)
	{
		delete world;
		world = new World(false);
		char *versionChar = new char();
		fscanf(worldFile, "%s ", versionChar);
		int worldBlocksX, worldBlocksY;
		fscanf(worldFile, "%d %d ", &worldBlocksX, &worldBlocksY);
		if (worldBlocksX != World::WIDTH || worldBlocksY != World::HEIGHT || strcmp(versionChar, VERSION_STRING) != 0)
		{
			bool exit;
			if (strcmp(versionChar, VERSION_STRING) != 0)
				exit = !createDialog(std::string("Incorrect World Version! Attempt load?"));
			else
				exit = !createDialog(std::string("Incorrect Block Count! Attempt load?"));
			delete versionChar;
			if (exit)
			{
				fclose(worldFile);
				return false;
			}
			clearText();
			drawBackground();
		}
		int loadWorld;
		fscanf(worldFile, "%d ", &loadWorld);
		world->gameMode = World::Mode(loadWorld);
		int loadTimeInWorld;
		fscanf(worldFile, "%d ", &loadTimeInWorld);
		world->timeInWorld = loadTimeInWorld;
		for (int i = 0; i < worldBlocksX; ++i)
		{
			for (int j = 0; j < worldBlocksY; ++j)
			{
				fscanf(worldFile, "%hd %hd %d ", &world->blocks[i][j], &world->bgblocks[i][j], &world->data[i][j]);
				if (perpetualUpdates(world->bgblocks[i][j]))
					updateSingleBlock(*world, i, j, true);
				if (perpetualUpdates(world->blocks[i][j]))
					updateSingleBlock(*world, i, j, false);
			}
			if (i % 64 == 0)
				printf("\x1b[22;33HLoading... %d%%", int(100 * (double(i) / double(World::WIDTH))));

		}
		calculateBrightness(*world);
		int loadBiome;
		for (int i = 0; i < World::WIDTH; ++i)
		{
			fscanf(worldFile, "%d ", &loadBiome);
			world->biome[i] = Biome(loadBiome);
		}
		loadInventory(worldFile);
		loadMobs(worldFile);
		loadChests(worldFile, *world);
		loadFurnaces(worldFile, *world);
		fscanf(worldFile, "%d ", &world->reservedWater);
		printf("\x1b[22;33H              ");
		fclose(worldFile);
		playMusic(MUSIC_CALM);
		fclose(worldFile);
		return true;
	}
	playMusic(MUSIC_CALM);
	return false;
}

bool saveConfig(Config *controls)
{
	FILE *fp;
	if (!SHOULD_SAVE) return false;
	if ((fp = fopen(MINE_DS_FOLDER CONTROLS_FILENAME, "w+")) != nullptr)
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
		fprintf(fp, "Gradient: %s\n", controls->getProperty(PROPERTY_GRADIENT) ? "Enabled" : "Disabled");
		fprintf(fp, "Dithering: %s\n", controls->getProperty(PROPERTY_DITHERING) ? "Enabled" : "Disabled");
		fprintf(fp, "Regeneration: %s\n", controls->getProperty(PROPERTY_REGEN) ? "Enabled" : "Disabled");
		fprintf(fp, "\nTexture Pack: %s\n", controls->textureName.c_str());
		fprintf(fp, "\n==Audio==\n");
		fprintf(fp, "Music Volume: %d\n", controls->musicVolume);
		fprintf(fp, "Sfx Volume: %d\n", controls->sfxVolume);
		fprintf(fp, "\n==Language==\n");
		fprintf(fp, "Language ID: %d\n", controls->languageID);
		fclose(fp);
		return true;
	}
	return false;
}

bool loadConfig(Config *controls)
{
	if (!SHOULD_LOAD) return false;
	FILE *fp;

	if ((fp = fopen(MINE_DS_FOLDER CONTROLS_FILENAME, "r")) != nullptr)
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
		fscanf(fp, "Gradient: %s\n", &parseChar);
		controls->setProperty(PROPERTY_GRADIENT, parsePropertyChar(&parseChar));
		fscanf(fp, "Dithering: %s\n", &parseChar);
		controls->setProperty(PROPERTY_DITHERING, parsePropertyChar(&parseChar));
		fscanf(fp, "Regeneration: %s\n", &parseChar);
		controls->setProperty(PROPERTY_REGEN, parsePropertyChar(&parseChar));
		fscanf(fp, "\nTexture Pack: %s\n", &parseChar);
		controls->textureName = &parseChar;
		fscanf(fp, "\n==Audio==\n");
		fscanf(fp, "Music Volume: %d\n", &controls->musicVolume);
		fscanf(fp, "Sfx Volume: %d\n", &controls->sfxVolume);
		fscanf(fp, "\n==Language==\n");
		fscanf(fp, "Language ID: %d\n", &controls->languageID);
		fclose(fp);	
		return true;
	}
	return false;
}

bool loadTexture(const char *fileName)
{
	if (!SHOULD_LOAD)
	{
		loadDefaultTexture();
		updateTexture();
	}
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
		blockTilesMem = nullptr;
		blockPalMem = nullptr;
	}

	unsigned int *mobTilesMem = new unsigned int[TILES_ARRAY_LEN];
	unsigned short *mobPalMem = new unsigned short[PAL_ARRAY_LEN];
	if (fread(mobTilesMem, sizeof (uint32_t), MOB_TILES_ARRAY_LEN, texFile) != MOB_TILES_ARRAY_LEN || fread(mobPalMem, sizeof (unsigned short), MOB_PAL_ARRAY_LEN, texFile) != MOB_PAL_ARRAY_LEN)
	{
		delete[] mobTilesMem;
		delete[] mobPalMem;
		mobTilesMem = nullptr;
		mobPalMem = nullptr;
	}

	unsigned int *subBgTilesMem = new unsigned int[TILES_ARRAY_LEN];
	unsigned short *subBgPalMem = new unsigned short[PAL_ARRAY_LEN];
	if (fread(subBgTilesMem, sizeof (uint32_t), TILES_ARRAY_LEN, texFile) != TILES_ARRAY_LEN || fread(subBgPalMem, sizeof (unsigned short), PAL_ARRAY_LEN, texFile) != PAL_ARRAY_LEN)
	{
		delete[] subBgTilesMem;
		delete[] subBgPalMem;
		subBgTilesMem = nullptr;
		subBgPalMem = nullptr;
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
