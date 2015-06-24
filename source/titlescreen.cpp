#include <time.h>
#include <dirent.h>
#include <string>
#include "deathScreen.h"
#include "graphics/subBgHandler.h"
#include "graphics/graphics.h"
#include "graphics/Button.h"
#include "mainGame.h"
#include "sounds.h"
#include "inventory.h"
#include "graphics/inventoryGraphics.h"
#include <stdio.h>
#include "Config.h"
#include "general.h"
#include "files.h"
#include "nifi.h"
#include "graphics/UI.h"
#include "daynight.h"
//Single Player/Multiplayer :D
//By the time we reach the title screen, all setup procedures should have been completed!
bool firstWorld = true;

void creditsScreen()
{
	createDialog(std::string("--- Programming ---\nCoolAs, Ray, Dirbaio, and Wolfgange\n\n--- Texture Packs ---\nMaxPack by Maxim\nAnd\nScary Sauce Pack\nby cool_story_bro\n\n--- Audio/Sounds ---\nSnowSong Pack\nby Alecia Shepherd"), false);
}

KEYPAD_BITS askForKeyScreen()
{
	consoleClear();
	const short X = 10, Y = 8;
	printXY(X + 1, Y + 1, "Up");
	printXY(X + 1, Y + 2, "Down");
	printXY(X + 1, Y + 3, "Left");
	printXY(X + 1, Y + 4, "Right");
	printXY(X + 1, Y + 5, "A");
	printXY(X + 1, Y + 6, "B");
	printXY(X + 1, Y + 7, "X");
	printXY(X + 1, Y + 8, "Y");
	printXY(X + 1, Y + 9, "L");
	printXY(X + 1, Y + 10, "R");
	printXY(X + 1, Y + 11, "Start");
	printXY(X + 1, Y + 12, "Select");
	KEYPAD_BITS key = KEY_LID;

	switch (listMenu(X, Y, 12, 6))
	{
		case 1:
			key = KEY_UP;
			break;
		case 2:
			key = KEY_DOWN;
			break;
		case 3:
			key = KEY_LEFT;
			break;
		case 4:
			key = KEY_RIGHT;
			break;
		case 5:
			key = KEY_A;
			break;
		case 6:
			key = KEY_B;
			break;
		case 7:
			key = KEY_X;
			break;
		case 8:
			key = KEY_Y;
			break;
		case 9:
			key = KEY_L;
			break;
		case 10:
			key = KEY_R;
			break;
		case 11:
			key = KEY_START;
			break;
		case 12:
			key = KEY_SELECT;
			break;
	}
	return key;
}

int getTappedAction(int column) //A dirty way of finding which action was tapped
{
	//===Regex===
	//Find: #define ([^ ]*) ([0-9]*)
	//Replace: case \2: return \1; break;
	switch (column)
	{
		case 1:
			return ACTION_MOVE_LEFT;
			break;
		case 2:
			return ACTION_MOVE_RIGHT;
			break;
		case 3:
			return ACTION_JUMP;
			break;
		case 4:
			return ACTION_CROUCH;
			break;
		case 5:
			return ACTION_ITEM_LEFT;
			break;
		case 6:
			return ACTION_ITEM_RIGHT;
			break;
		case 7:
			return ACTION_SWITCH_SCREEN;
			break;
		case 8:
			return ACTION_MENU;
			break;
		case 9:
			return ACTION_CLIMB;
			break;
		case 10:
			return ACTION_DROP;
			break;
		default:
			return -1;
	}
}

void texturePackScreen()
{
	consoleClear();
	const short X = 5, Y = 9, MAX_NAME_LENGTH = 20;
	DIR *textureDir = opendir(MINE_DS_FOLDER TEXTURE_FOLDER);
	struct dirent *dirContents;
	int numItems = 0;
	printXY(X + 1, Y + 1, "Max Pack (Default)");
	if (textureDir)
	{
		while ((dirContents = readdir(textureDir)) != NULL)
		{
			if (numItems > 1) //Removes the /. and /..
			{
				std::string s(dirContents->d_name);
				s.erase(s.find_last_of("."), std::string::npos);
				s.resize(MAX_NAME_LENGTH);
				printXY(X + 1, Y + numItems, s.c_str());
			}
			++numItems;
		}
		closedir(textureDir);
	}
	else
		numItems = 2;
	int fileNum = listMenu(X, Y, numItems - 1, MAX_NAME_LENGTH);
	switch (fileNum)
	{
		case 0:
			return;
		case 1:
			loadDefaultTexture();
			updateTexture();
			drawWorld();
			return;
		default:
			break;
	}
	++fileNum; //To remove the /..
	textureDir = opendir(MINE_DS_FOLDER TEXTURE_FOLDER);
	numItems = 1;
	if (textureDir)
	{
		while (fileNum > numItems && readdir(textureDir) != NULL)
			++numItems;
		dirContents = readdir(textureDir);
		closedir(textureDir);
		getGlobalSettings()->textureName = dirContents->d_name;
		bool loadError = !loadTexture(dirContents->d_name);
		drawWorld();
		if (loadError)
			createDialog(std::string("Cannot load Texture! Loading the Default texture pack."), false);
	}
}

void setControlsScreen()
{
	while (1)
	{
	consoleClear();
	const short X = 8, Y = 9;
	printXY(X + 1, Y + 1, "Move Left");
	printXY(X + 1, Y + 2, "Move Right");
	printXY(X + 1, Y + 3, "Jump");
	printXY(X + 1, Y + 4, "Crouch");
	printXY(X + 1, Y + 5, "Item Left");
	printXY(X + 1, Y + 6, "Item Right");
	printXY(X + 1, Y + 7, "Switch Screen");
	printXY(X + 1, Y + 8, "Menu");
	printXY(X + 1, Y + 9, "Climb");
	printXY(X + 1, Y + 10, "Drop Item");
	int tapped = getTappedAction(listMenu(X, Y, 10, 13));
	if (tapped == -1)
			break;
	KEYPAD_BITS key = askForKeyScreen();
	if (key != KEY_LID)
		getGlobalSettings()->setKey(tapped, key);
	}
}

void viewControls()
{
	drawBackground();
	consoleClear();
	const short ITEMS = 10;
	const short X = 4, Y = 7;
	const short MAX_LENGTH = 13 + 3 + 5;

	drawBox(X, Y, MAX_LENGTH + 2, ITEMS + 2);
	printXY(X + 1, Y + 1, "    Move Left---");
	printXY(X + 1, Y + 2, "   Move Right---");
	printXY(X + 1, Y + 3, "         Jump---");
	printXY(X + 1, Y + 4, "       Crouch---");
	printXY(X + 1, Y + 5, "    Item Left---");
	printXY(X + 1, Y + 6, "   Item Right---");
	printXY(X + 1, Y + 7, "Switch Screen---");
	printXY(X + 1, Y + 8, "         Menu---");
	printXY(X + 1, Y + 9, "        Climb---");
	printXY(X + 1, Y + 10, "    Drop Item---");

	printXY(X + 17, Y + 1, getKeyChar(getGlobalSettings()->getKey(ACTION_MOVE_LEFT)));
	printXY(X + 17, Y + 2, getKeyChar(getGlobalSettings()->getKey(ACTION_MOVE_RIGHT)));
	printXY(X + 17, Y + 3, getKeyChar(getGlobalSettings()->getKey(ACTION_JUMP)));
	printXY(X + 17, Y + 4, getKeyChar(getGlobalSettings()->getKey(ACTION_CROUCH)));
	printXY(X + 17, Y + 5, getKeyChar(getGlobalSettings()->getKey(ACTION_ITEM_LEFT)));
	printXY(X + 17, Y + 6, getKeyChar(getGlobalSettings()->getKey(ACTION_ITEM_RIGHT)));
	printXY(X + 17, Y + 7, getKeyChar(getGlobalSettings()->getKey(ACTION_SWITCH_SCREEN)));
	printXY(X + 17, Y + 8, getKeyChar(getGlobalSettings()->getKey(ACTION_MENU)));
	printXY(X + 17, Y + 9, getKeyChar(getGlobalSettings()->getKey(ACTION_CLIMB)));
	printXY(X + 17, Y + 10, getKeyChar(getGlobalSettings()->getKey(ACTION_DROP)));
	Button buttons[0];
	menu(buttons, 0);
}

void controlsScreen()
{
	bool exit = false;

	while (!exit)
	{
		consoleClear();
		drawBackground();

		Button view(10, 10, "View", 12);
		Button edit(10, 16, "Edit", 12);
		Button buttons[] = {edit, view};

		switch (menu(buttons, 2))
		{
			case 1: //Edit controls
				setControlsScreen();
				break;
			case 2: //View controls
				viewControls();
				break;
			default: //Back
				exit = true;
				break;
		}
	}
	saveControls(getGlobalSettings());
}

void gameOptions()
{
	bool exit = false;

	while (!exit)
	{
		consoleClear();
		drawBackground();

		Button herobrine(8, 7, "Herobrine", 14);
		Button drawMode(8, 11, "Draw Mode", 14);
		Button creativeSpeed(7, 15, "Creative Speed", 16);
		Button smoothcamera(7, 19, "Smooth camera", 16);
		Button buttons[] = {herobrine, drawMode, creativeSpeed, smoothcamera};

		switch (menu(buttons, 4))
		{
			case 1: // herobrine
				getGlobalSettings()->setProperty(PROPERTY_HEROBRINE, enableDisableMenu(getGlobalSettings()->getProperty(PROPERTY_HEROBRINE)));
				break;
			case 2: // draw mode
				getGlobalSettings()->setProperty(PROPERTY_DRAW, enableDisableMenu(getGlobalSettings()->getProperty(PROPERTY_DRAW)));
				break;
			case 3: // creative speed
				getGlobalSettings()->setProperty(PROPERTY_SPEED, enableDisableMenu(getGlobalSettings()->getProperty(PROPERTY_SPEED)));
				break;
			case 4: // Smooth camera
				getGlobalSettings()->setProperty(PROPERTY_SMOOTH, enableDisableMenu(getGlobalSettings()->getProperty(PROPERTY_SMOOTH)));
				break;
			default: // back button
				exit = true;
				break;
		}
	}
}

void graphicsScreen()
{
	bool exit = false;
	while (!exit)
	{
		consoleClear();
		drawBackground();

		Button texture(8, 8, "Texture Pack", 15);
		Button gradient(8, 13, "Sky Gradient", 15);
		Button dithering(8, 18, "Sky Dithering", 15, getGlobalSettings()->getProperty(PROPERTY_GRADIENT));
		Button buttons[] = {texture, gradient, dithering};

		switch (menu(buttons, 3, true, getGlobalSettings()->getProperty(PROPERTY_GRADIENT)))
		{
			case 1:
				texturePackScreen();
				break;
			case 2:
				getGlobalSettings()->setProperty(PROPERTY_GRADIENT, enableDisableMenu(getGlobalSettings()->getProperty(PROPERTY_GRADIENT)));
				setSkyDay();
				break;
			case 3:
				getGlobalSettings()->setProperty(PROPERTY_DITHERING, enableDisableMenu(getGlobalSettings()->getProperty(PROPERTY_DITHERING)));
				setSkyDay();
				break;
			default:
				exit = true;
				break;
		}
	}
}

void settingsScreen()
{
	bool exit = false;

	while (!exit)
	{
		consoleClear();
		drawBackground();

		Button controls(8, 8, "Controls", 15);
		Button options(8, 13, "Game Options", 15);
		Button texture(8, 18, "Texture Pack", 15);
		Button gradient(8, 23, "Sky Gradient", 15);
		Button dithering(8, 28, "Sky Dithering", 15, getGlobalSettings()->getProperty(PROPERTY_GRADIENT));
		Button buttons[] = {controls, options, texture, gradient, dithering};

		switch (menu(buttons, 5, true, getGlobalSettings()->getProperty(PROPERTY_GRADIENT) ? 64 : 30))
		{
			case 1: // controls
				controlsScreen();
				break;
			case 2: // game options
				gameOptions();
				break;
			case 3: // Graphics options
				texturePackScreen();
				break;
			case 4:
				getGlobalSettings()->setProperty(PROPERTY_GRADIENT, enableDisableMenu(getGlobalSettings()->getProperty(PROPERTY_GRADIENT)));
				setSkyDay();
				break;
			case 5:
				getGlobalSettings()->setProperty(PROPERTY_DITHERING, enableDisableMenu(getGlobalSettings()->getProperty(PROPERTY_DITHERING)));
				setSkyDay();
				break;
			default: // back button
				exit = true;
				break;
		}
	}
}

void gameModeScreen()
{
	drawBackground();
	consoleClear();

	Button creativeButton(9, 8, "Creative", 12);
	Button survivalButton(9, 13, "Survival", 12);
	Button loadButton(9, 18, "Load World", 12);
	Button buttons[] = {creativeButton, survivalButton, loadButton};

	switch (menu(buttons, 3))
	{
		case 1: // creative mode
			// TODO: Add menu to set game seed
			printXY(1, 22, "Generating creative game");
			newGame(GAMEMODE_CREATIVE, 0);
			break;
		case 2: // survival mode
			// TODO: Add menu to set game seed
			printXY(1, 22, "Generating survival game");
			newGame(GAMEMODE_SURVIVAL, 0);
			break;
		case 3: // load game
			printXY(1, 22, "Loading game");
			if (!loadGame())
			{
				printXY(1, 22, "Failed to load game");
				sleepThread(1);
				return;
			}
			break;
		default: // back button
			return;
	}
	startGame();
	firstWorld = false;
}

void multiplayerScreen()
{
	consoleClear();
	drawBackground();

	Button create(9, 8, "Create Game", 13);
	Button load(9, 13, "Load Game", 13);
	Button join(9, 18, "Join Game", 13);
	Button buttons[] = {create, load, join};

	switch (menu(buttons, 3))
	{
		case 1: // create game
			startMultiplayerGame(true);
			break;
		case 2: // TODO: load game
			startMultiplayerGame(true);
			break;
		case 3: // join game
			joinGame();
			break;
		default: // back button
			return;
	}
}

void titlescreen()
{
	bool poweroff = false;

	previewGame();
	while (!poweroff)
	{
		lcdMainOnTop();
		drawBackground();
		consoleClear();
		playMusic(MUSIC_CALM);
		clearInventory(true);

		Button singlePlayer(8, 8, "Single Player", 15);
		Button settings(8, 13, "Settings", 15);
		Button credits(8, 18, "Credits", 15);
		Button power(29, 21, "\xFE"); // \xFE = Dot
		Button buttons[] = {singlePlayer, settings, credits, power};

		switch (menu(buttons, 4, false))
		{
			case 1: // single player
				gameModeScreen();
				break;
			case 2: // settings
				settingsScreen();
				break;
			case 3: //Credits
				creditsScreen();
				break;
			case 4: //Power
				poweroff = true;
				break;
		}
	}
}
