#include <nds.h>
#include <string>
#include <algorithm>
#include "Config.h"
#include "general.h"

Config globalSettings;

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
		case ACTION_DROP:
			dropItem = key;
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
		case ACTION_DROP:
			returnKey = dropItem;
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

Config *getGlobalSettings()
{
	return &globalSettings;
}

const char * getKeyChar(KEYPAD_BITS key)
{
	const char * returnKeyChar;
	switch (key)
	{
		case KEY_UP:
			returnKeyChar = "Up";
			break;
		case KEY_DOWN:
			returnKeyChar = "Down";
			break;
		case KEY_LEFT:
			returnKeyChar = "Left";
			break;
		case KEY_RIGHT:
			returnKeyChar = "Right";
			break;
		case KEY_A:
			returnKeyChar = "A";
			break;
		case KEY_B:
			returnKeyChar = "B";
			break;
		case KEY_X:
			returnKeyChar = "X";
			break;
		case KEY_Y:
			returnKeyChar = "Y";
			break;
		case KEY_L:
			returnKeyChar = "L";
			break;
		case KEY_R:
			returnKeyChar = "R";
			break;
		case KEY_START:
			returnKeyChar = "Start";
			break;
		case KEY_SELECT:
			returnKeyChar = "Select";
			break;
		default:
			returnKeyChar = "Error";
	}
	return returnKeyChar;
}

std::string stringToUpper(std::string convert)
{
	std::transform(convert.begin(), convert.end(), convert.begin(), ::toupper);

	return convert;
}

KEYPAD_BITS parseKeyChar(const char *input)
{
	std::string upperCase(input);
	upperCase = stringToUpper(upperCase);
	if (upperCase == "UP") return KEY_UP;
	else if (upperCase == "DOWN") return KEY_DOWN;
	else if (upperCase == "LEFT") return KEY_LEFT;
	else if (upperCase == "RIGHT") return KEY_RIGHT;
	else if (upperCase == "A") return KEY_A;
	else if (upperCase == "B") return KEY_B;
	else if (upperCase == "X") return KEY_X;
	else if (upperCase == "Y") return KEY_Y;
	else if (upperCase == "L") return KEY_L;
	else if (upperCase == "R") return KEY_R;
	else if (upperCase == "START") return KEY_START;
	else if (upperCase == "SELECT") return KEY_SELECT;
	else return KEY_A; //Random Key
}

bool parsePropertyChar(const char *input)
{
	std::string upperCase(input);
	upperCase = stringToUpper(upperCase);
	if (strcmp(upperCase.c_str(), "ENABLED") == 0)
		return true;
	if (strcmp(upperCase.c_str(), "DISABLED") == 0)
		return false;
	return NULL;
}
