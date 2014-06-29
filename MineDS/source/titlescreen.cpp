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
			if (back.isTouching(touch))
				back.setColored(true);
			for (int i = 0; i < size; i++)
				if (buttons[i].isTouching(touch))
					buttons[i].setColored(true);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (back.isColored && back.isTouching(touch))
			{
				selected = 0;
				chosen = true;
			}
			for (int i = 0; i < size; i++)
			{
				if (buttons[i].isColored && buttons[i].isTouching(touch))
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

void multiplayerScreen()
{
	consoleClear();
	drawBackground();

	Button create(9, 9, "Create Game", 12);
	Button join(9, 14, "Join Game", 12);
	Button buttons[] = {create, join};
	switch (menu(buttons, 2))
	{
		case 1:
		{
			survival = false;
			drawButtonColored(9, 9, 12);
			drawBackground();
			theWorld = multiplayerGame(true, theWorld);
			multiplayer = true;
			stopMusic();
			playCalm = false;
			playMusic(HAL2);
			playHal2 = true;
		}
		case 2:
		{
			survival = false;
			drawBackground();
			theWorld = multiplayerGame(false, theWorld);
			multiplayer = true;
			stopMusic();
			playCalm = false;
			playMusic(HAL2);
			playHal2 = true;
		}
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
			if (back.isTouching(touch))
				back.setColored(true);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (back.isColored && back.isTouching(touch))
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
			else if (back.isTouching(touch))
				back.setColored(true);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (back.isColored && back.isTouching(touch))
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
			else if (back.isTouching(touch))
				back.setColored(true);

		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (back.isColored && back.isTouching(touch))
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

	Button controls(10, 9, "Controls", 10);
	Button credits(10, 14, "Credits", 10);
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

int gameModeScreen()
{
	int returnVal = 2;
	drawBackground();
	consoleClear();
	Button creativeButton(9, 8, "Creative", 12);
	Button survivalButton(9, 13, "Survival", 12);
	Button loadButton(9, 18, "Load World", 12);
	Button buttons[] = {creativeButton, survivalButton, loadButton};

	switch (menu(buttons, 3))
	{
		case 1://Creative
		{
			clearInventory(); //TODO: Clear inventory on Death, not here
			survival = false;
			drawButtonColored(9, 9 - 1, 12);
			drawBackground();
			consoleClear();
			stopMusic();
			playCalm = false;
			playMusic(HAL2);
			playHal2 = true;
			updateInvGraphics();
			theWorld = mainGame(0, theWorld);
			gameGen = true;
			multiplayer = false;
			returnVal = 1;
			break;
		}
		case 2://Survival
		{
			clearInventory();
			survival = true;
			drawButtonColored(9, 14 - 1, 12);
			drawBackground();
			consoleClear();
			stopMusic();
			playCalm = false;
			playMusic(HAL2);
			playHal2 = true;
			updateInvGraphics();
			theWorld = mainGame(0, theWorld);
			gameGen = true;
			multiplayer = false;
			returnVal = 1;
			break;
		}
		case 3: //Load World
		{
			clearInventory();
			survival = true;
			drawButtonColored(9, 19 - 1, 12);
			drawBackground();
			consoleClear();
			stopMusic();
			playCalm = false;
			playMusic(HAL2);
			playHal2 = true;
			updateInvGraphics();
			theWorld = mainGame(2, theWorld);
			gameGen = true;
			multiplayer = false;
			returnVal = 1;
			break;
		}
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
	bool chosen = false;
	touchPosition touch;
	drawBackground();
	consoleClear();
	Button singlePlayer = Button(8, 9, "Single Player", 14);
	Button multiPlayer = Button(8, 14, "Multiplayer", 14);
	Button settings = Button(8, 19, "Settings", 14);
	Button back = Button(25, 19, "Back", gameGen && !multiplayer && !getDied());

	uint oldKeys = keysHeld();
	while (!chosen)
	{
		swiWaitForVBlank();
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH))
		{
			touchRead(&touch);
			if (singlePlayer.isTouching(touch))
				singlePlayer.setColored(true);
			else if (multiPlayer.isTouching(touch))
				multiPlayer.setColored(true);
			else if (settings.isTouching(touch))
				settings.setColored(true);
			else if (back.isTouching(touch))
				back.setColored(true);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (singlePlayer.isColored && singlePlayer.isTouching(touch))
			{
				switch (gameModeScreen()) //Switch is used to make it scalable
				{
					case 1://Start Game
						chosen = true;
						break;
					case 2://Back to Main Menu

						chosen = true;
						break;
				}
			}
			else singlePlayer.setColored(false);
			if (multiPlayer.isColored && multiPlayer.isTouching(touch))
			{
				multiplayerScreen();
				chosen = true;
			}
			else multiPlayer.setColored(false);
			if (settings.isColored && settings.isTouching(touch))
			{
				while (!settingsScreen());
				chosen = true;
			}
			else settings.setColored(false);
			if (back.isColored && back.isTouching(touch))
			{
				drawBackground();
				consoleClear();
				stopMusic();
				playCalm = false;
				playMusic(HAL2);
				playHal2 = true;
				theWorld = mainGame(1, theWorld);
				chosen = true;
			}
			else
				back.setColored(false); //Back button
		}
		oldKeys = keysHeld();
		touchRead(&touch);
	}
}

