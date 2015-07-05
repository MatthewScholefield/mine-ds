#pragma once
#include <nds.h>
#include <string>

#define ACTION_MOVE_LEFT 1
#define ACTION_MOVE_RIGHT 2
#define ACTION_JUMP 3
#define ACTION_CROUCH 4
#define ACTION_ITEM_LEFT 5
#define ACTION_ITEM_RIGHT 6
#define ACTION_SWITCH_SCREEN 7
#define ACTION_MENU 8
#define ACTION_CLIMB 9
#define ACTION_DROP 10

#define PROPERTY_HEROBRINE 0
#define PROPERTY_DRAW 1
#define PROPERTY_SPEED 2
#define PROPERTY_SMOOTH 3
#define PROPERTY_GRADIENT 4
#define PROPERTY_DITHERING 5

class Config
{
private:
	KEYPAD_BITS left;
	KEYPAD_BITS right;
	KEYPAD_BITS jump;
	KEYPAD_BITS crouch;
	KEYPAD_BITS itemLeft;
	KEYPAD_BITS itemRight;
	KEYPAD_BITS switchScreen;
	KEYPAD_BITS menu;
	KEYPAD_BITS climb;
	KEYPAD_BITS dropItem;
	bool properties[6];
public:
  int sfxVolume;
  int soundVolume;
	std::string textureName;
	void setKey(int action, KEYPAD_BITS key);
	bool getProperty(int property);
	void setProperty(int property, bool);
	KEYPAD_BITS getKey(int action);

	Config() : left(KEY_LEFT),
	right(KEY_RIGHT),
	jump(KEY_A),
	crouch(KEY_DOWN),
	itemLeft(KEY_X),
	itemRight(KEY_B),
	switchScreen(KEY_Y),
	menu(KEY_START),
	climb(KEY_UP),
	dropItem(KEY_SELECT),
	properties
	{
		false, false, true, true, true, true
	}

	,sfxVolume(20),soundVolume(25),
	textureName("default") { }
};

Config *getGlobalSettings();
const char * getKeyChar(KEYPAD_BITS key);
KEYPAD_BITS parseKeyChar(const char *input);
bool parsePropertyChar(const char *input);
