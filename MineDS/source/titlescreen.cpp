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

	int i, j; // They are famous variables :P
	int k = 90;
	int l = 122;
	for (i = 0; i <= 24; i = i + 2)
	{
		for (j = 0; j <= 31; j++)
		{
			setSubBgTile(j, i, k); //Draws dirt Background
			if (k == 90)
				k++;
			else if (k != 90)
				k = 90;
		}
	}
	for (i = -1; i <= 24; i = i + 2)
	{
		for (j = 0; j <= 31; j++)
		{
			setSubBgTile(j, i, l);
			if (l == 122)
				l++;
			else if (l != 122)
				l = 122;
		}
	}
	for (i = 0; i <= 25; i++)
		for (j = 0; j <= 6; j++)
			setSubBgTile(i + 2, j, i + (j * 32)); //Draw the MineDS Logo!
}

int menu(Button buttons[], int size)
{
	touchPosition touch;
	printXY(26, 20, "Back");
	drawButton(25, 19, 4); //Back button
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
			if (gameGen && !multiplayer && !getDied() && touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				drawButtonColored(25, 19, 4);
			else
			{
				for (int i = 0; i < size; i++)
					if (buttons[i].isTouching(touch))
						buttons[i].setColored(true);
			}
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (gameGen && !multiplayer && !getDied() && touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			{
				selected = 0;
				chosen = true;
			}
			else
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
				drawButton(25, 19, 4); //Back button
				for (int i = 0; i < size; i++)
					buttons[i].setColored(false);
			}
		}
		oldKeys = keysHeld();
		touchRead(&touch);
	}
	return selected;
}

void multiplayerScreen()
{
	uint oldKeys;
	touchPosition touch;

	drawBackground();
	drawButton(9, 9, 12);
	drawButton(9, 14, 12);
	drawButton(25, 19, 4); //Back button
	consoleClear(); //Remove All text from the screen
	printXY(10, 10, "Create Game");
	printXY(11, 15, "Join Game");
	printXY(26, 20, "Back");
	scanKeys();
	oldKeys = keysHeld();
	swiWaitForVBlank(); // Get "newKeys"
	bool chosen = false;
	while (!chosen)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
				drawButtonColored(9, 9, 12);
			else if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
				drawButtonColored(9, 14, 12);
			else if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				drawButtonColored(25, 19, 4);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
			{
				survival = true;
				drawButtonColored(9, 9, 12);
				drawBackground();
				theWorld = multiplayerGame(true, theWorld);
				multiplayer = true;
				stopMusic();
				playCalm = false;
				playMusic(HAL2);
				playHal2 = true;
				chosen = true;
			}
			else drawButton(9, 9, 12);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{
				survival = true;
				drawBackground();
				theWorld = multiplayerGame(false, theWorld);
				multiplayer = true;
				stopMusic();
				playCalm = false;
				playMusic(HAL2);
				playHal2 = true;
				chosen = true;
			}
			else drawButton(9, 14, 12);
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			{
				chosen = true;
			}
			else drawButton(25, 19, 4);
		}
		oldKeys = keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
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
	drawButton(25, 19, 4); //Back button
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
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				drawButtonColored(25, 19, 4);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			{
				chosen = true;
			}
			else drawButton(25, 19, 4);
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

	//Draw Buttons
	drawButton(25, 19, 4); //Back button

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
	printXY(26, 20, "Back");


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
			else if (touch.px > 25 * 8 && touch.px < 30 * 8 && touch.py > 19 * 8 && touch.py < 22 * 8)
				drawButtonColored(25, 19, 4); //Back button
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
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
				drawButton(25, 19, 4); //Back button
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
	drawButton(25, 19, 4); //Back button

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
			else if (touch.px > 25 * 8 && touch.px < 30 * 8 && touch.py > 19 * 8 && touch.py < 22 * 8)
				drawButtonColored(25, 19, 4); //Back button

		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				return true;
			else if (column <= ITEMS && column >= 1 && touch.px >= (X + 1) * 8 && touch.px < (X + MAX_NAME_LENGTH + 1) * 8)
			{
				setKey(getTappedAction(column), askForKeyScreen());
				return false;
			}
			else //Remove any colored buttons, if any
			{
				drawBoxCenter(X + 1, Y + 1, MAX_NAME_LENGTH, ITEMS);
				drawButton(25, 19, 4); //Back button
			}
		}
		oldKeys = keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}

bool settingsScreen()
{
	uint oldKeys;
	touchPosition touch;

	drawBackground();
	drawButton(10, 9, 10);
	drawButton(10, 14, 10);
	drawButton(25, 19, 4); //Back button
	consoleClear(); //Removes All text from the screen
	printXY(12, 10, "Controls");
	printXY(12, 15, "Credits");
	printXY(26, 20, "Back");
	scanKeys();
	oldKeys = keysHeld();
	swiWaitForVBlank(); // Get "newKeys"
	while (1)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
				drawButtonColored(10, 9, 10);
			else if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
				drawButtonColored(10, 14, 10);
			else if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				drawButtonColored(25, 19, 4);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
			{
				while (!controlsScreen());
				return false;
			}
			else drawButton(10, 9, 10);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{
				creditsScreen();
				return false;
			}
			else drawButton(10, 14, 10);
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				return true;
			else drawButton(25, 19, 4);
		}
		oldKeys = keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
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
			break;
		}
	}
}

int titlescreen()
{
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
	bool redraw = true, chosen = false;
	while (redraw)
	{
		redraw = false;
		touchPosition touch;
		lcdMainOnTop();
		drawBackground();
		consoleClear();
		Button singlePlayer = Button(8, 9, "Single Player", 14);
		Button multiPlayer = Button(8, 14, "Multiplayer", 14);
		Button settings = Button(8, 19, "Settings", 14);

		if (gameGen && !multiplayer && !getDied())
		{
			printXY(26, 20, "Back");
			drawButton(25, 19, 4); //Back button
		}

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
				else if (gameGen && !multiplayer && !getDied() && touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
					drawButtonColored(25, 19, 4);
			}
			else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
			{
				if (singlePlayer.isColored && singlePlayer.isTouching(touch))
				{
					gameModeScreen();
					chosen = true;
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
					redraw = true;
					chosen = true;
				}
				else settings.setColored(false);
				if (gameGen && !multiplayer && !getDied() && touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
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
				else if (gameGen && !multiplayer && !getDied())
					drawButton(25, 19, 4); //Back button
			}
			oldKeys = keysHeld();
		}
	}
	return 0;
}

