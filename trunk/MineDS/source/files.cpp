#include <fat.h>
#include <nds.h>
#include <stdio.h>
#include <string>
#include "general.h"
#include "Config.h"
#include "world.h"
#include "inventory.h"
#include "controls.h"

FILE* data;
FILE* controlData;

bool init = false;

void stopNow()
{
	while (1)
		swiWaitForVBlank();
}

void closeFiles(bool control = false)
{
	fclose(control ? controlData : data);
}

bool endOfFile()
{
	return feof(data);
}

bool openControlFiles(const char* mode = "rb")
{
	if (!init)
	{
		fatInitDefault();
		init = true;
	}
	controlData = fopen("/mineds/config.bin", mode);
	if (!controlData)
	{
		printXY(1, 22, "Error opening /mineds/config.bin");
		for (int i = 0; i < 80; i++)
			swiWaitForVBlank();
		return false;
	}
	return true;
}

bool openFiles(const char* mode)
{
	if (!init)
	{
		fatInitDefault();
		init = true;
	}
	data = fopen("/mineds/data.bin", mode);
	if (!data)
	{
		printXY(1, 22, "Error opening /mineds/data.bin");
		for (int i = 0; i < 80; i++)
			swiWaitForVBlank();
		return false;
	}
	return true;
}

bool openFiles()
{
	return openFiles("rb");
}

std::string getLine()
{
	char buf[1000];
	fgets(buf, 1000, data);
	std::string temp(buf);
	return temp;
}

const char getChar()
{
	return fgetc(data);
}

void writeString(std::string write)
{
	for (char i : write)
		fputc(i, data);
}

bool saveWorld(worldObject* world)
{
	const bool success = openFiles("wb");
	if (success)
	{
		//world is the world object
		//write the world object
		fwrite(world, sizeof (*world), 1, data);
		saveInventory(data);
		closeFiles();
	}
	return success;
}

void saveControls()
{
	if (openControlFiles("wb"))
	{
		fseek(controlData, 0, SEEK_SET);
		Config* settings = getSettings();
		fwrite(settings, sizeof (*settings), 1, controlData);
		//saveControlData(controlData);
		closeFiles(true);
		printXY(1, 22, "Saved Controls");
		for (int i = 0; i < 80; i++)
			swiWaitForVBlank();
	}
}

void loadControls()
{
	if (openControlFiles())
	{
		Config* settings = getSettings();
		fseek(controlData, 0, SEEK_SET);
		fread(settings, sizeof (*settings), 1, controlData);
		//loadControlData(controlData);
		closeFiles(true);
		printXY(1, 22, "Loaded Controls");
		for (int i = 0; i < 80; i++)
			swiWaitForVBlank();
	}
}

void loadWorld(worldObject* world)
{
	if (openFiles())
	{
		//world is the world object
		fread(world, sizeof (*world), 1, data);
		loadInventory(data);
		closeFiles();
	}
}
