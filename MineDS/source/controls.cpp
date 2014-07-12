#include <nds.h>
#include <stdio.h>
#include "Config.h"

Config controls;

KEYPAD_BITS getControlKey(int key)
{
	return controls.getKey(key);
}

void setControlKey(int action, KEYPAD_BITS key)
{
	controls.setKey(action, key);
}

void setHerobrineOn(bool on)
{
	controls.herobrineOn = on;
}

bool getHerobrineOn()
{
	return controls.herobrineOn;
}

void setDrawMode(bool on)
{
	controls.drawMode = on;
}

bool getDrawMode()
{
	return controls.drawMode;
}

void saveControlData(FILE* data)
{
	fwrite(&controls, sizeof (controls), 1, data);
}

void loadControlData(FILE* data)
{
	fread(&controls, sizeof (controls), 1, data);
}