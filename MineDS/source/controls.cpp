#include <nds.h>
#include <stdio.h>
#include "Config.h"

Config controls;

KEYPAD_BITS getControlKey(int key)
{
	return controls.getKey(key);
}

Config* getSettings()
{
	return &controls;
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