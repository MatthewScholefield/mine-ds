#include "deathScreen.h"
#include "graphics/subBgHandler.h"
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

void settings(); //forward statements
int titlescreen();
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
	//lcdMainOnTop();
	drawBackground();
	//Draw Buttons
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
	;
	touchPosition touch;
	//lcdMainOnTop();
	drawBackground();
	drawButton(11, 8, 7);
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
	KEYPAD_BITS key = KEY_START;
	uint oldKeys;
	touchPosition touch;
	//lcdMainOnTop();
	drawBackground();
	//Draw Buttons
	drawButton(8, 14, 13);
	drawButton(9, 9, 11);
	drawButton(25, 19, 4); //Back button
	consoleClear(); //Removes All text from the screen
	printXY(0, 0, "Up");
	printXY(0, 1, "Down");
	printXY(0, 2, "Left");
	printXY(0, 3, "Right");
	printXY(0, 4, "A");
	printXY(0, 5, "B");
	printXY(0, 6, "X");
	printXY(0, 7, "Y");
	printXY(0, 8, "L");
	printXY(0, 9, "R");
	printXY(0, 10, "Start");
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
			//if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			//	drawButtonColored(25,19,4);
			switch (((touch.py - 8) / 8) + 2)
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
			printXY(1, 1, "Done 1");
			chosen = true;
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			/*if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
			{
				drawButtonColored(9,9,11);
				drawButton(8,14,13);
			}
			else drawButton(9,9,11);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{
				drawButton(9,9,11);
				drawButtonColored(8,14,13);
			}
			else drawButton(8,14,13);*/
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			{
				chosen = true; //Will return to previous function when chosen is true.
				//No need to call settings...
			}
			else drawButton(25, 19, 4);
		}
		oldKeys = keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
	return key;
}

int getTappedAction(int y)
{
	int column = ((y - 8) / 8) + 2;
	//Regex
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

void controls()
{
	uint oldKeys;
	touchPosition touch;
	//lcdMainOnTop();
	drawBackground();
	//Draw Buttons
	drawButton(8, 14, 13);
	drawButton(9, 9, 11);
	drawButton(25, 19, 4); //Back button
	consoleClear(); //Removes All text from the screen
	printXY(0, 0, "Move Left");
	printXY(0, 1, "Move Right");
	printXY(0, 2, "Jump");
	printXY(0, 3, "Crouch");
	printXY(0, 4, "Item Left");
	printXY(0, 5, "Item Right");
	printXY(0, 6, "Switch Screen");
	printXY(0, 7, "Menu");
	printXY(0, 8, "Climb");
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
			//if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			//	drawButtonColored(25,19,4);
			setKey(getTappedAction(touch.py), askForKey());
			chosen = true;
		}
		/*else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
			{
				drawButtonColored(9,9,11);
				drawButton(8,14,13);
			}
			else drawButton(9,9,11);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{
				drawButton(9,9,11);
				drawButtonColored(8,14,13);
			}
			else drawButton(8,14,13);
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			{
				chosen = true;	//Will return to previous function when chosen is true.
				//No need to call settings...
			}
			else drawButton(25,19,4);
		}*/
		oldKeys = keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}

void settings_redraw()
{
	drawBackground();
	//Draw Buttons
	drawButton(10, 9, 10);
	drawButton(10, 14, 10);
	drawButton(25, 19, 4); //Back button
	consoleClear(); //Removes All text from the screen
	printXY(12, 10, "Controls");
	printXY(12, 15, "Credits");
	printXY(26, 20, "Back");
}

void settings()
{
	uint oldKeys;
	touchPosition touch;
	//lcdMainOnTop();
	drawBackground();
	//Draw Buttons
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
	bool chosen = false;
	while (!chosen)
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
				controls();
				settings_redraw();
			}
			else drawButton(10, 9, 10);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{
				credits();
				settings_redraw();
			}
			else drawButton(10, 14, 10);
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

void gameMode()
{
	uint oldKeys;
	touchPosition touch;
	//lcdMainOnTop();
	drawBackground();
	//Draw Buttons
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
				clearInventory();
				survival = false;
				drawButtonColored(9, 9 - 1, 12);
				;
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
	//Lets start the buttons on line 8!
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
	uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	drawBackground();
	//Lets start the buttons on line 8!
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
				settings();
				titlescreen_redraw();
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
	return 0;
}
