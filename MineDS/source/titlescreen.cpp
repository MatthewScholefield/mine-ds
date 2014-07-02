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
#include "general.h"
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
		{
			if (i % 2)
				setSubBgTile(j, i, 90 + j % 2);
			else
				setSubBgTile(j, i, 122 + j % 2);
		}
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
	return selected + start;
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
	drawButton(11, 8, 8);
	drawButton(9, 12, 11);
	drawButton(5, 16, 21);
	drawButton(7, 20, 16);
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

	const short ITEMS = 11;
	const short MAX_NAME_LENGTH = 5;
	const short X = 11, Y = 8;

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

bool controlsScreen()
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
				setKey(getTappedAction(column), askForKeyScreen());
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

bool settingsScreen()
{
	consoleClear();
	drawBackground();

	Button controls(10, 10, "Controls", 10);
	Button credits(10, 15, "Credits", 10);
	Button buttons[] = {controls, credits};
	switch (menu(buttons, 2))
	{
		case 1:
		{
			while (!controlsScreen());
			return false;
		}
		case 2:
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

