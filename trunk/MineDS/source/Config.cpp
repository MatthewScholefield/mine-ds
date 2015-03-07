#include <nds.h>
#include "Config.h"

Config globalSettings;

Config *getGlobalSettings()
{
	return &globalSettings;
}

void Config::setKey(int action, KEYPAD_BITS key)
{
	switch (action)
	{
		case ACTION_MOVE_LEFT:
			left = key;
			break;
		case ACTION_MOVE_RIGHT:
			right = key;
			break;
		case ACTION_JUMP:
			jump = key;
			break;
		case ACTION_CROUCH:
			crouch = key;
			break;
		case ACTION_ITEM_LEFT:
			itemLeft = key;
			break;
		case ACTION_ITEM_RIGHT:
			itemRight = key;
			break;
		case ACTION_SWITCH_SCREEN:
			switchScreen = key;
			break;
		case ACTION_MENU:
			menu = key;
			break;
		case ACTION_CLIMB:
			climb = key;
			break;
	}

}

KEYPAD_BITS Config::getKey(int action)
{
	KEYPAD_BITS returnKey = KEY_LID;
	switch (action)
	{
		case ACTION_MOVE_LEFT:
			returnKey = left;
			break;
		case ACTION_MOVE_RIGHT:
			returnKey = right;
			break;
		case ACTION_JUMP:
			returnKey = jump;
			break;
		case ACTION_CROUCH:
			returnKey = crouch;
			break;
		case ACTION_ITEM_LEFT:
			returnKey = itemLeft;
			break;
		case ACTION_ITEM_RIGHT:
			returnKey = itemRight;
			break;
		case ACTION_SWITCH_SCREEN:
			returnKey = switchScreen;
			break;
		case ACTION_MENU:
			returnKey = menu;
			break;
		case ACTION_CLIMB:
			returnKey = climb;
			break;
	}
	return returnKey;
}

void Config::setProperty(int property, bool set)
{
	properties[property] = set;
}

bool Config::getProperty(int property)
{
	return properties[property];
}