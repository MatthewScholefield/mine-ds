#include <fat.h>
#include <nds.h>
#include <stdio.h>
//#include <sys/dir.h>
#include <string>
#include "general.h"
#include "world.h"
#include "inventory.h"
#include "controls.h"

FILE* data;

bool init = false;

void stopNow()
{
	while (1)
		swiWaitForVBlank();
}

void closeFiles()
{
	fclose(data);
}

bool endOfFile()
{
	return feof(data);
}

bool openFiles(const char* mode)
{
	if (!init)
	{
		fatInitDefault();
		init = true;
	}
	data = fopen("/mineds/data.txt", mode);
	if (!data)
	{
		printXY(1, 22, "Error opening /mineds/data.txt");
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
	if (openFiles("wb"))
	{
		saveControlData(data);
		closeFiles();
	}
}

void loadControls()
{
	if (openFiles())
	{
		loadControlData(data);
		closeFiles();
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
