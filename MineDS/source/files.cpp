#include <fat.h>
#include <nds.h>
#include <stdio.h>
#include <string>
#include "general.h"
#include "Config.h"
#include "world.h"
#include "inventory.h"

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

bool openControlFiles(bool silent, const char* mode = "rb")
{
	if (!init)
	{
		fatInitDefault();
		init = true;
	}
	controlData = fopen("/MineConfig.bin", mode);
	if (!controlData && !silent)
	{
		printXY(1, 22, "Error opening /MineConfig.bin");
		for (int i = 0; i < 80; i++)
			swiWaitForVBlank();
	}
	if (!controlData)
		return false;
	return true;
}

bool openFiles(const char* mode = "rb")
{
	if (!init)
	{
		fatInitDefault();
		init = true;
	}
	data = fopen("/MineData.bin", mode);
	if (!data)
	{
		printXY(1, 22, "Error opening /MineData.bin");
		for (int i = 0; i < 80; i++)
			swiWaitForVBlank();
		return false;
	}
	return true;
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
	const bool success = openFiles("wb+");
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

bool saveControls(bool silent)
{
	if (openControlFiles(silent, "wb+"))
	{
		fseek(controlData, 0, SEEK_SET);
		fwrite(getGlobalSettings(), sizeof (*getGlobalSettings()), 1, controlData);
		//saveControlData(controlData);
		closeFiles(true);
		if (!silent)
		{
			printXY(1, 22, "Saved Controls");
			for (int i = 0; i < 80; i++)
				swiWaitForVBlank();
		}
		return true;
	}
	return false;
}

bool loadControls(bool silent)
{
	if (openControlFiles(silent))
	{
		fseek(controlData, 0, SEEK_SET);
		fread(getGlobalSettings(), sizeof (*getGlobalSettings()), 1, controlData);
		//loadControlData(controlData);
		closeFiles(true);
		if (!silent)
		{
			printXY(1, 22, "Loaded Controls");
			for (int i = 0; i < 80; i++)
				swiWaitForVBlank();
		}
		return true;
	}
	return false;
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
