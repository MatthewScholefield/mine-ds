#include "deathScreen.h"
#include "graphics/subBgHandler.h"
#include "graphics/graphics.h"
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
			{
				k++;
			}
			else if (k != 90)
			{
				k = 90;
			}
		}
	}
	for (i = -1; i <= 24; i = i + 2)
	{
		for (j = 0; j <= 31; j++)
		{
			setSubBgTile(j, i, l);
			if (l == 122)
			{
				l++;
			}
			else if (l != 122)
			{
				l = 122;
			}
		}
	}
	for (i = 0; i <= 25; i++)
		for (j = 0; j <= 6; j++)
			setSubBgTile(i + 2, j, i + (j * 32)); //Draw the MineDS Logo!
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

void credits()
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

KEYPAD_BITS askForKey()
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

bool controls()
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
				setKey(getTappedAction(column), askForKey());
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

bool settings()
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
				while (!controls());
				return false;
			}
			else drawButton(10, 9, 10);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{
				credits();
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

void gameMode()
{
	uint oldKeys;
	touchPosition touch;

	drawBackground();
	drawButton(9, 9 - 1, 12);
	drawButton(9, 14 - 1, 12);
	drawButton(9, 19 - 1, 12);
	drawButton(25, 20 - 1, 4); //Back button
	consoleClear(); //Removes All text from the screen
	printXY(12, 9, "Creative"); //x:12 y:9
	printXY(12, 14, "Survival"); //x:12 y:14
	printXY(11, 19, "Load World"); //x:11 y:19
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
			if (touch.px > 72 && touch.px < 176 && touch.py > (9 - 1)*8 && touch.py < (9 + 2)*8)
				drawButtonColored(9, 9 - 1, 12); //Creative
			else if (touch.px > 72 && touch.px < 176 && touch.py > (14 - 1)*8 && touch.py < (14 + 2)*8)
				drawButtonColored(9, 14 - 1, 12); //Survival
			else if (touch.px > 72 && touch.px < 176 && touch.py > (19 - 1)*8 && touch.py < (19 + 2)*8)
				drawButtonColored(9, 19 - 1, 12); //Load World
			else if (touch.px > 200 && touch.px < 240 && touch.py > (20 - 1)*8 && touch.py < (20 + 2)*8)
				drawButtonColored(25, 20 - 1, 4); //Back
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 72 && touch.px < 176 && touch.py > (9 - 1)*8 && touch.py < (9 + 2)*8)//Creative
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
				chosen = true;
			}
			else drawButton(9, 9 - 1, 12);
			;
			if (touch.px > 72 && touch.px < 176 && touch.py > (14 - 1)*8 && touch.py < (14 + 2)*8)//Survival
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
				chosen = true;
			}
			else drawButton(9, 14 - 1, 12);
			if (touch.px > 72 && touch.px < 176 && touch.py > 144 && touch.py < 168) //Load World
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
				chosen = true;
			}
			else
				drawButton(9, 19 - 1, 12);
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			{
				chosen = true;
			}
			else drawButton(25, 20 - 1, 4);
		}
		oldKeys = keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}

void titlescreen_redraw()
{
	drawBackground();
	drawButton(8, 9, 14);
	drawButton(8, 14, 14);
	drawButton(8, 19, 14);

	if (gameGen && !multiplayer && !getDied())
		drawButton(25, 19, 4); //Back button

	consoleClear();
	printXY(9, 10, "Single Player");
	printXY(10, 15, "Multi Player");
	printXY(12, 20, "Settings");
	if (gameGen && !multiplayer && !getDied())
		printXY(26, 20, "Back");
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
	bool redraw = true;
	while (redraw)
	{
		redraw = false;
		uint oldKeys;
		touchPosition touch;
		lcdMainOnTop();
		drawBackground();

		drawButton(8, 9, 14);
		drawButton(8, 14, 14);
		drawButton(8, 19, 14);
		if (gameGen && !multiplayer && !getDied())
			drawButton(25, 19, 4); //Back button
		//Clear the screen!
		consoleClear();
		//Print the Buttons
		printXY(9, 10, "Single Player");
		printXY(10, 15, "Multi Player");
		printXY(12, 20, "Settings");
		if (gameGen && !multiplayer && !getDied())
			printXY(26, 20, "Back");
		bool chosen = false;
		scanKeys();
		oldKeys = keysHeld();
		swiWaitForVBlank();
		while (!chosen)
		{
			scanKeys();
			if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH))
			{
				touchRead(&touch);
				//Check if over SinglePlayerButton
				if (touch.px > 64 && touch.px < 184 && touch.py > 72 && touch.py < 96)
					drawButtonColored(8, 9, 14);
				else if (touch.px > 64 && touch.px < 184 && touch.py > 112 && touch.py < 136)
					drawButtonColored(8, 14, 14);
				else if (touch.px > 64 && touch.px < 184 && touch.py > 152 && touch.py < 176)
					drawButtonColored(8, 19, 14);
				else if (gameGen && !multiplayer && !getDied() && touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
					drawButtonColored(25, 19, 4);
			}
			else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
			{
				if (touch.px > 64 && touch.px < 184 && touch.py > 72 && touch.py < 96)
				{
					gameMode();
					chosen = true;
				}
				else drawButton(8, 9, 14);
				if (touch.px > 64 && touch.px < 184 && touch.py > 112 && touch.py < 136)
				{
					multiplayerScreen();
					chosen = true;
				}
				else drawButton(8, 14, 14);
				if (touch.px > 64 && touch.px < 184 && touch.py > 152 && touch.py < 176)
				{
					while (!settings());
					redraw = true;
					chosen = true;
				}
				else drawButton(8, 19, 14);
				if (gameGen && !multiplayer && !getDied() && touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				{
					drawButtonColored(8, 9, 14);
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
			touchRead(&touch);
			swiWaitForVBlank();
		}
	}
	return 0;
}
