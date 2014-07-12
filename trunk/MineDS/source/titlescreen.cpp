#include "deathScreen.h"
#include "graphics/subBgHandler.h"
#include "graphics/graphics.h"
#include "graphics/Button.h"
#include "mainGame.h"
#include "multiplayerGame.h"
#include "sounds.h"
#include "inventory.h"
#include "graphics/inventoryGraphics.h"
#include <stdio.h>
#include "controls.h"
#include "Config.h"
#include "general.h"
#include "files.h"
//Single Player/Multiplayer :D
//By the time we reach the title screen, all setup procedures should have been completed!

bool playCalm = false; // Whether CALM music is playing
bool playHal2 = false; // Whether Hal2 music is playing
bool gameGen = false; //Whether world has been generated and the back key will allow going back to it
bool multiplayer = false; //Whether generated world is multiplayer
bool survival; // Whether Game Mode is Survival
worldObject* theWorld;

bool isSurvival()
{
	return survival;
}

void drawBackground() //Draws dirt background and MineDS Logo
{

	int i, j; //They are famous variables :P

	for (i = 0; i <= 24; i++) //Draws dirt Background
		for (j = 0; j <= 31; j++)
			setSubBgTile(j, i, (i % 2 ? 90 : 122) + j % 2);
	for (i = 0; i <= 25; i++)
		for (j = 0; j <= 6; j++)
			setSubBgTile(i + 2, j, i + (j * 32)); //Draw the MineDS Logo!
}

int menu(Button buttons[], int size, bool showBack)
{
	int start = 0;
	Button back(25, 19, "Back", showBack);
	touchPosition touch;
	int selected = -1;
	bool chosen = false;

	uint oldKeys = keysHeld();
	while (!chosen)
	{
		swiWaitForVBlank();
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH))
		{
			touchRead(&touch);
			if (back.isTouching(touch.px, touch.py))
				back.setColored(true);
			for (int i = 0; i < size; i++)
				if (buttons[i].isTouching(touch.px, touch.py))
					buttons[i].setColored(true);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (back.isColored && back.isTouching(touch.px, touch.py))
			{
				selected = 0;
				chosen = true;
			}
			for (int i = 0; i < size; i++)
			{
				if (buttons[i].isColored && buttons[i].isTouching(touch.px, touch.py))
				{
					selected = i + 1;
					chosen = true;
				}
			}
			if (!chosen) //Redraw buttons
			{
				back.setColored(false);
				for (int i = 0; i < size; i++)
					buttons[i].setColored(false);
			}
		}
		oldKeys = keysHeld();
		touchRead(&touch);
	}
	for (int i = 0; i < size; i++)
		buttons[i].setColored(false);
	return selected + start;
}

bool enableDisableMenu(bool initial)
{
	uint oldKeys;
	touchPosition touch;
	drawBackground();

	consoleClear(); //Removes All text from the screen
	Button enabled(9, 10, "Enabled", 12);
	Button disabled(9, 15, "Disabled", 12);
	Button done(25, 19, "Done");
	enabled.setColored(initial);
	disabled.setColored(!initial);

	scanKeys();
	touchRead(&touch);
	oldKeys = keysHeld();
	swiWaitForVBlank();
	bool returnVal = initial;
	while (1)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			if (enabled.isTouching(touch.px, touch.py))
			{
				returnVal = true;
				enabled.setColored(true);
				disabled.setColored(false);
			}
			else if (disabled.isTouching(touch.px, touch.py))
			{
				returnVal = false;
				enabled.setColored(false);
				disabled.setColored(true);
			}
			done.setColored(done.isTouching(touch.px, touch.py));
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (done.isColored && done.isTouching(touch.px, touch.py))
				return returnVal;
		}
		oldKeys = keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}

int menu(Button buttons[], int size)
{
	return menu(buttons, size, true);
}

void startMultiplayer(bool host)
{
	survival = false;
	multiplayer = true;
	drawBackground();
	theWorld = multiplayerGame(host, theWorld);
	stopMusic();
	playCalm = false;
	playMusic(HAL2);
	playHal2 = true;
}

void multiplayerScreen()
{
	consoleClear();
	drawBackground();

	Button create(9, 10, "Create Game", 13);
	Button join(9, 15, "Join Game", 13);
	Button buttons[] = {create, join};
	switch (menu(buttons, 2))
	{
		case 1:startMultiplayer(true);
			break;
		case 2:startMultiplayer(false);
			break;
	}
}

void creditsScreen()
{
	uint oldKeys = keysHeld();
	touchPosition touch;
	drawBackground();
	drawButton(11, 8, 9);
	drawButton(9, 12, 12);
	drawButton(5, 16, 22);
	drawButton(7, 20, 17);
	Button back(25, 19, "Back");
	consoleClear();
	printXY(12, 9, "Mine DS");
	printXY(11, 13, "Coded by");
	printXY(6, 17, "CoolAs, Ray, Dirbaio,");
	printXY(9, 21, "and Wolfgange");
	printXY(26, 20, "Back");
	scanKeys();
	bool chosen = false;
	while (!chosen)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			if (back.isTouching(touch.px, touch.py))
				back.setColored(true);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (back.isColored && back.isTouching(touch.px, touch.py))
				chosen = true;
			else back.setColored(false);
		}
		oldKeys = keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}

KEYPAD_BITS askForKeyScreen()
{
	drawBackground();
	consoleClear(); //Removes All text from the screen

	Button back(25, 19, "Back");

	const short ITEMS = 12;
	const short MAX_NAME_LENGTH = 6;
	const short X = 10, Y = 8;

	drawBox(X, Y, MAX_NAME_LENGTH + 2, ITEMS + 2);
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


	uint oldKeys;
	touchPosition touch;
	scanKeys();
	touchRead(&touch);
	oldKeys = keysHeld();
	swiWaitForVBlank();
	KEYPAD_BITS key = KEY_LID;
	bool chosen = false;

	while (!chosen)
	{
		scanKeys();
		int column = ((touch.py - 8) / 8) + 1 - Y;
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			column = ((touch.py - 8) / 8) + 1 - Y;
			if (column <= ITEMS && column >= 1 && touch.px >= (X + 1) * 8 && touch.px < (X + MAX_NAME_LENGTH + 1) * 8)
				for (int i = 0; i < MAX_NAME_LENGTH; i++)
					setSubBgTile(X + 1 + i, Y + column, 60);
			else if (back.isTouching(touch.px, touch.py))
				back.setColored(true);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (back.isColored && back.isTouching(touch.px, touch.py))
				chosen = true;
			else if (column <= ITEMS && column >= 1 && touch.px >= (X + 1) * 8 && touch.px < (X + MAX_NAME_LENGTH + 1) * 8)
			{
				switch (column)
				{
					case 1: key = KEY_UP;
						break;
					case 2: key = KEY_DOWN;
						break;
					case 3: key = KEY_LEFT;
						break;
					case 4: key = KEY_RIGHT;
						break;
					case 5: key = KEY_A;
						break;
					case 6: key = KEY_B;
						break;
					case 7: key = KEY_X;
						break;
					case 8: key = KEY_Y;
						break;
					case 9: key = KEY_L;
						break;
					case 10: key = KEY_R;
						break;
					case 11: key = KEY_START;
						break;
					case 12: key = KEY_SELECT;
						break;
				}
				chosen = true;
			}
			else //Remove any colored buttons, if any
			{
				drawBoxCenter(X + 1, Y + 1, MAX_NAME_LENGTH, ITEMS);
				back.setColored(false);
			}
		}
		oldKeys = keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
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
		case 1: return ACTION_MOVE_LEFT;
			break;
		case 2: return ACTION_MOVE_RIGHT;
			break;
		case 3: return ACTION_JUMP;
			break;
		case 4: return ACTION_CROUCH;
			break;
		case 5: return ACTION_ITEM_LEFT;
			break;
		case 6: return ACTION_ITEM_RIGHT;
			break;
		case 7: return ACTION_SWITCH_SCREEN;
			break;
		case 8: return ACTION_MENU;
			break;
		case 9: return ACTION_CLIMB;
			break;
		default: return -1;
	}
}

bool setControlsScreen()
{
	uint oldKeys;
	touchPosition touch;
	drawBackground();

	Button back(25, 19, "Back");

	consoleClear(); //Removes All text from the screen
	const short ITEMS = 9;
	const short MAX_NAME_LENGTH = 13;
	const short X = 8, Y = 9;

	drawBox(X, Y, MAX_NAME_LENGTH + 2, ITEMS + 2);
	printXY(X + 1, Y + 1, "Move Left");
	printXY(X + 1, Y + 2, "Move Right");
	printXY(X + 1, Y + 3, "Jump");
	printXY(X + 1, Y + 4, "Crouch");
	printXY(X + 1, Y + 5, "Item Left");
	printXY(X + 1, Y + 6, "Item Right");
	printXY(X + 1, Y + 7, "Switch Screen");
	printXY(X + 1, Y + 8, "Menu");
	printXY(X + 1, Y + 9, "Climb");
	printXY(26, 20, "Back");

	scanKeys();
	touchRead(&touch);
	oldKeys = keysHeld();
	swiWaitForVBlank();
	while (1)
	{
		scanKeys();
		int column = ((touch.py - 8) / 8) + 1 - Y;
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			column = ((touch.py - 8) / 8) + 1 - Y;
			if (column <= ITEMS && column >= 1 && touch.px >= (X + 1) * 8 && touch.px < (X + MAX_NAME_LENGTH + 1) * 8)
				for (int i = 0; i < MAX_NAME_LENGTH; i++)
					setSubBgTile(X + 1 + i, Y + column, 60);
			else if (back.isTouching(touch.px, touch.py))
				back.setColored(true);

		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (back.isColored && back.isTouching(touch.px, touch.py))
				return true;
			else if (column <= ITEMS && column >= 1 && touch.px >= (X + 1) * 8 && touch.px < (X + MAX_NAME_LENGTH + 1) * 8)
			{
				setControlKey(getTappedAction(column), askForKeyScreen());
				return false;
			}
			else //Remove any colored buttons, if any
			{
				drawBoxCenter(X + 1, Y + 1, MAX_NAME_LENGTH, ITEMS);
				back.setColored(false);
			}
		}
		oldKeys = keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}

const char * getKeyChar(KEYPAD_BITS key)
{
	const char * returnKeyChar;
	switch (key)
	{
		case KEY_UP: returnKeyChar = "Up";
			break;
		case KEY_DOWN: returnKeyChar = "Down";
			break;
		case KEY_LEFT: returnKeyChar = "Left";
			break;
		case KEY_RIGHT: returnKeyChar = "Right";
			break;
		case KEY_A: returnKeyChar = "A";
			break;
		case KEY_B: returnKeyChar = "B";
			break;
		case KEY_X: returnKeyChar = "X";
			break;
		case KEY_Y: returnKeyChar = "Y";
			break;
		case KEY_L: returnKeyChar = "L";
			break;
		case KEY_R: returnKeyChar = "R";
			break;
		case KEY_START: returnKeyChar = "Start";
			break;
		case KEY_SELECT: returnKeyChar = "Select";
		default: returnKeyChar = "Error";
	}
	return returnKeyChar;
}

void viewControls()
{
	drawBackground();
	consoleClear();
	const short ITEMS = 9;
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

	printXY(X + 17, Y + 1, getKeyChar(getControlKey(ACTION_MOVE_LEFT)));
	printXY(X + 17, Y + 2, getKeyChar(getControlKey(ACTION_MOVE_RIGHT)));
	printXY(X + 17, Y + 3, getKeyChar(getControlKey(ACTION_JUMP)));
	printXY(X + 17, Y + 4, getKeyChar(getControlKey(ACTION_CROUCH)));
	printXY(X + 17, Y + 5, getKeyChar(getControlKey(ACTION_ITEM_LEFT)));
	printXY(X + 17, Y + 6, getKeyChar(getControlKey(ACTION_ITEM_RIGHT)));
	printXY(X + 17, Y + 7, getKeyChar(getControlKey(ACTION_SWITCH_SCREEN)));
	printXY(X + 17, Y + 8, getKeyChar(getControlKey(ACTION_MENU)));
	printXY(X + 17, Y + 9, getKeyChar(getControlKey(ACTION_CLIMB)));
	Button buttons[0];
	menu(buttons, 0);
}

bool controlsScreen()
{
	consoleClear();
	drawBackground();

	Button view(8, 10, "View");
	Button edit(17, 10, "Edit");
	Button save(8, 15, "Save");
	Button load(17, 15, "Load");

	Button buttons[] = {edit, save, load, view};
	switch (menu(buttons, 4))
	{
		case 0: return true;
		case 1: while (!setControlsScreen());
			return false;
		case 2: saveControls();
			return false;
		case 3: saveControls();
			return false;
		case 4: viewControls();
		default: return false;
	}
}

bool gameOptions()
{
	consoleClear();
	drawBackground();

	Button herobrine(8, 10, "Herobrine", 14);
	Button drawMode(8, 15, "Draw Mode", 14);
	Button buttons[] = {herobrine, drawMode};
	switch (menu(buttons, 2))
	{
		case 1:
		{
			setHerobrineOn(enableDisableMenu(getHerobrineOn()));
			return false;
		}
		case 2:
		{
			setDrawMode(enableDisableMenu(getDrawMode()));
			return false;
		}
		default:
			return true;
	}
}

bool settingsScreen()
{
	consoleClear();
	drawBackground();

	Button controls(8, 8, "Controls", 15);
	Button options(8, 13, "Game Options", 15);
	Button credits(8, 18, "Credits", 15);
	Button buttons[] = {controls, options, credits};
	switch (menu(buttons, 3))
	{
		case 1:
		{
			while (!controlsScreen());
			return false;
		}
		case 2:
		{
			while (!gameOptions());
			return false;
		}
		case 3:
		{
			creditsScreen();
			return false;
		}
		default: return true;
	}
}

void startSingleplayer(bool setSurvival, bool load)
{
	clearInventory(); //TODO: Clear inventory on Death, not here
	survival = setSurvival;
	drawBackground();
	consoleClear();
	stopMusic();
	playCalm = false;
	playMusic(HAL2);
	playHal2 = true;
	updateInvGraphics();
	theWorld = mainGame(load ? 2 : 0, theWorld);
	gameGen = true;
	multiplayer = false;
}

int gameModeScreen()
{
	int returnVal = 1;
	drawBackground();
	consoleClear();
	Button creativeButton(9, 8, "Creative", 12);
	Button survivalButton(9, 13, "Survival", 12);
	Button loadButton(9, 18, "Load World", 12);
	Button buttons[] = {creativeButton, survivalButton, loadButton};

	switch (menu(buttons, 3))
	{
		case 0: returnVal = 2;
			break;
		case 1: startSingleplayer(false, false);
			break;
		case 2: startSingleplayer(true, false);
			break;
		case 3: startSingleplayer(true, true);
			break;
	}
	return returnVal;
}

void titlescreen()
{
	lcdMainOnTop();
	if (theWorld == NULL)
	{
		theWorld = (worldObject *) calloc(1, sizeof (worldObject));
		theWorld = mainGame(3, theWorld);
	}
	if (playHal2)
	{
		stopMusic();
		playHal2 = false;
	}
	if (!playCalm)
	{
		playMusic(CALM);
		playCalm = true;
	}

	drawBackground();
	consoleClear();

	Button singlePlayer = Button(8, 8, "Single Player", 15);
	Button multiPlayer = Button(8, 13, "Multiplayer", 15);
	Button settings = Button(8, 18, "Settings", 15);
	Button buttons[] = {singlePlayer, multiPlayer, settings};

	switch (menu(buttons, 3, gameGen && !multiplayer && !getDied()))
	{
		case 0: //back
		{
			drawBackground();
			consoleClear();
			stopMusic();
			playCalm = false;
			playMusic(HAL2);
			playHal2 = true;
			theWorld = mainGame(1, theWorld);
			break;
		}
		case 1: gameModeScreen();
			break;
		case 2: multiplayerScreen();
			break;
		case 3: while (!settingsScreen());
			break;
	}
}

