#include "deathScreen.h"
#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "world.h"
#include "mainGame.h"
#include "multiplayerGame.h"
#include "sounds.h"
#include "nifi.h"
#include "inventory.h"
#include "graphics/inventoryGraphics.h"
#include <nds.h>
#include <stdio.h>
#include "controls.h"
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

	int i,j; // They are famous variables :P
	int k=90;
	int l=122;
	for (i=0;i<=24;i=i+2)
	{
		for (j=0;j<=31;j++)
		{
			setSubBgTile(j,i,k);//Draws dirt Background
			if (k == 90)
			{
				k++;
			}
			else if (k!=90)
			{
				k=90;
			}
		}
	}
	for (i=-1;i<=24;i=i+2)
	{
		for (j=0;j<=31;j++)
		{
			setSubBgTile(j,i,l);
			if (l == 122)
			{
				l++;
			}
			else if (l != 122)
			{
				l=122;
			}
		}
	}
	for (i=0;i<=25;i++)
		for (j=0;j<=6;j++)
			setSubBgTile(i+2,j,i+(j*32)); //Draw the MineDS Logo!
}

void multiplayerScreen()
{
	uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	drawBackground();
	//Draw Buttons
	drawButton(9,9,12);
	drawButton(9,14,12);
	drawButton(25,19,4);//Back button
	consoleClear(); //Remove All text from the screen
	iprintf("\x1b[10;10HCreate Game");
	iprintf("\x1b[15;11HJoin Game");
	iprintf("\x1b[20;26HBack");
	scanKeys();
	oldKeys=keysHeld();
	swiWaitForVBlank(); // Get "newKeys"
	bool chosen=false;
	while (!chosen)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
				drawButtonColored(9,9,12);
			else if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
				drawButtonColored(9,14,12);
			else if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				drawButtonColored(25,19,4);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
			{
				survival = true;
				drawButtonColored(9,9,12);
				drawBackground();
				theWorld = multiplayerGame(true,theWorld);
				multiplayer = true;
				stopMusic();
				playCalm = false;
				playMusic(HAL2);
				playHal2 = true;
				chosen=true;
			}
			else drawButton(9,9,12);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{
				survival = true;
				drawBackground();
				theWorld = multiplayerGame(false,theWorld);
				multiplayer = true;
				stopMusic();
				playCalm = false;
				playMusic(HAL2);
				playHal2 = true;
				chosen=true;
			}
			else drawButton(9,14,12);
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			{
				chosen = true;
			}
			else drawButton(25,19,4);
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}
void credits()
{
	uint oldKeys=keysHeld();;
	touchPosition touch;
	lcdMainOnTop();
	drawBackground();
	drawButton(11,8,7);
	drawButton(9,12,11);
	drawButton(5,16,21);
	drawButton(7,20,16);
	drawButton(25,19,4);//Back button
	consoleClear();
	iprintf("\x1b[9;12HMine DS");
	iprintf("\x1b[13;11HCoded by");
	iprintf("\x1b[17;6HCoolAs, Ray, Dirbaio,");
	iprintf("\x1b[21;9Hand Wolfgange");
	iprintf("\x1b[20;26HBack");
	scanKeys();
	bool chosen=false;
	while (!chosen)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				drawButtonColored(25,19,4);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			{
				chosen = true;
			}
			else drawButton(25,19,4);
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}

KEYPAD_BITS askForKey()
{
	KEYPAD_BITS key = KEY_START;
	uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	drawBackground();
	//Draw Buttons
	drawButton(8,14,13);
	drawButton(9,9,11);
	drawButton(25,19,4); //Back button
	consoleClear(); //Removes All text from the screen
	iprintf("\x1b[0;0HUp");
	iprintf("\x1b[1;0HDown");
	iprintf("\x1b[2;0HLeft");
	iprintf("\x1b[3;0HRight");
	iprintf("\x1b[4;0HA");
	iprintf("\x1b[5;0HB");
	iprintf("\x1b[6;0HX");
	iprintf("\x1b[7;0HY");
	iprintf("\x1b[8;0HL");
	iprintf("\x1b[9;0HR");
	iprintf("\x1b[10;0HStart");
	iprintf("\x1b[20;26HBack");
	scanKeys();
	oldKeys=keysHeld();
	swiWaitForVBlank(); // Get "newKeys"
	bool chosen=false;
	while (!chosen)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			//if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			//	drawButtonColored(25,19,4);
			switch (((touch.py-8)/8)+2)
			{
				case 1: key = KEY_UP; break;
				case 2: key = KEY_DOWN; break;
				case 3: key = KEY_LEFT; break;
				case 4: key = KEY_RIGHT; break;
				case 5: key = KEY_A; break;
				case 6: key = KEY_B; break;
				case 7: key = KEY_X; break;
				case 8: key = KEY_Y; break;
				case 9: key = KEY_L; break;
				case 10: key = KEY_R; break;
				case 11: key = KEY_START; break;
				case 12: key = KEY_SELECT; break;
			}
			iprintf("\x1b[1;1HDone 1");
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
				chosen = true;	//Will return to previous function when chosen is true.
				//No need to call settings...
			}
			else drawButton(25,19,4);
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
	return key;
}

int getTappedAction(int y)
{
	int column = ((y-8)/8)+2;
	//Regex
	//Find: #define ([^ ]*) ([0-9]*)
	//Replace: case \2: return \1; break;
	switch (column)
	{
		case 1: return ACTION_MOVE_LEFT; break;
		case 2: return ACTION_MOVE_RIGHT; break;
		case 3: return ACTION_JUMP; break;
		case 4: return ACTION_CROUCH; break;
		case 5: return ACTION_ITEM_LEFT; break;
		case 6: return ACTION_ITEM_RIGHT; break;
		case 7: return ACTION_SWITCH_SCREEN; break;
		case 8: return ACTION_MENU; break;
		case 9: return ACTION_CLIMB; break;
		default: return -1;
	}
}

void controls()
{
	uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	drawBackground();
	//Draw Buttons
	drawButton(8,14,13);
	drawButton(9,9,11);
	drawButton(25,19,4); //Back button
	consoleClear(); //Removes All text from the screen
	iprintf("\x1b[0;0HMove Left");
	iprintf("\x1b[1;0HMove Right");
	iprintf("\x1b[2;0HJump");
	iprintf("\x1b[3;0HCrouch");
	iprintf("\x1b[4;0HItem Left");
	iprintf("\x1b[5;0HItem Right");
	iprintf("\x1b[6;0HSwitch Screen");
	iprintf("\x1b[7;0HMenu");
	iprintf("\x1b[8;0HClimb");
	iprintf("\x1b[20;26HBack");
	scanKeys();
	oldKeys=keysHeld();
	swiWaitForVBlank(); // Get "newKeys"
	bool chosen=false;
	while (!chosen)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			//if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			//	drawButtonColored(25,19,4);
			setKey(getTappedAction(touch.py),askForKey());
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
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}

void settings_redraw()
{
	drawBackground();
	//Draw Buttons
	drawButton(10,9,10);
	drawButton(10,14,10);
	drawButton(25,19,4); //Back button
	consoleClear(); //Removes All text from the screen
	iprintf("\x1b[10;12HControls");
	iprintf("\x1b[15;12HCredits");
	iprintf("\x1b[20;26HBack");
}
void settings()
{
	uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	drawBackground();
	//Draw Buttons
	drawButton(10,9,10);
	drawButton(10,14,10);
	drawButton(25,19,4); //Back button
	consoleClear(); //Removes All text from the screen
	iprintf("\x1b[10;12HControls");
	iprintf("\x1b[15;12HCredits");
	iprintf("\x1b[20;26HBack");
	scanKeys();
	oldKeys=keysHeld();
	swiWaitForVBlank(); // Get "newKeys"
	bool chosen=false;
	while (!chosen)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
				drawButtonColored(10,9,10);
			else if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
				drawButtonColored(10,14,10);
			else if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				drawButtonColored(25,19,4);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
			{
				controls();
				settings_redraw();
			}
			else drawButton(10,9,10);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{
				credits();
				settings_redraw();
			}
			else drawButton(10,14,10);
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			{
				chosen = true;
			}
			else drawButton(25,19,4);
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}
void gameMode()
{
	uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	drawBackground();
	//Draw Buttons
	drawButton(10,9,10);
	drawButton(10,14,10);
	drawButton(25,19,4); //Back button
	consoleClear(); //Removes All text from the screen
	iprintf("\x1b[10;12HCreative");
	iprintf("\x1b[15;12HSurvival");
	iprintf("\x1b[20;26HBack");
	scanKeys();
	oldKeys=keysHeld();
	swiWaitForVBlank(); // Get "newKeys"
	bool chosen=false;
	while (!chosen)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			touchRead(&touch);
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
				drawButtonColored(10,9,10);
			else if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
				drawButtonColored(10,14,10);
			else if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				drawButtonColored(25,19,4);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
			{
				clearInventory();
				survival = false;
				drawButtonColored(8,9,14);
				drawBackground();
				consoleClear();
				stopMusic();
				playCalm = false;
				playMusic(HAL2);
				playHal2 = true;
				updateInvGraphics();
				theWorld = mainGame(0,theWorld);
				gameGen = true;
				multiplayer = false;
				chosen=true;
			}
			else drawButton(10,9,10);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{
				clearInventory();
				survival = true;
				drawButtonColored(8,9,14);
				drawBackground();
				consoleClear();
				stopMusic();
				playCalm = false;
				playMusic(HAL2);
				playHal2 = true;
				updateInvGraphics();
				theWorld = mainGame(0,theWorld);
				gameGen = true;
				multiplayer = false;
				chosen=true;
			}
			else drawButton(10,14,10);
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			{
				chosen = true;
			}
			else drawButton(25,19,4);
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}
void titlescreen_redraw()
{
	drawBackground();
	//Lets start the buttons on line 8!
	drawButton(8,9,14);
	drawButton(8,14,14);
	drawButton(8,19,14);
	if (gameGen && !multiplayer && !getDied())
		drawButton(25,19,4); //Back button
	//Clear the screen!
	consoleClear();
	//Print the Buttons
	iprintf("\x1b[10;9HSingle Player");
	iprintf("\x1b[15;10HMulti Player");
	iprintf("\x1b[20;12HSettings");
	if (gameGen && !multiplayer && !getDied())
		iprintf("\x1b[20;26HBack");
}
int titlescreen()
{
	if (theWorld==NULL) theWorld = (worldObject *) calloc(1, sizeof(worldObject));
	if (playHal2)
	{
		stopMusic();
		playHal2 = false;
	}
	if (!playCalm)
	{
		playMusic (CALM);
		playCalm = true;
	}
	uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	drawBackground();
	//Lets start the buttons on line 8!
	drawButton(8,9,14);
	drawButton(8,14,14);
	drawButton(8,19,14);
	if (gameGen && !multiplayer && !getDied())
		drawButton(25,19,4); //Back button
	//Clear the screen!
	consoleClear();
	//Print the Buttons
	iprintf("\x1b[10;9HSingle Player");
	iprintf("\x1b[15;10HMulti Player");
	iprintf("\x1b[20;12HSettings");
	if (gameGen && !multiplayer && !getDied())
		iprintf("\x1b[20;26HBack");
	bool chosen=false;
	scanKeys();
	oldKeys=keysHeld();
	swiWaitForVBlank();	
	while (!chosen)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH))
		{
			touchRead(&touch);
			//Check if over SinglePlayerButton
			if (touch.px > 64 && touch.px < 184 && touch.py > 72 && touch.py < 96)
				drawButtonColored(8,9,14);
			else if (touch.px > 64 && touch.px < 184 && touch.py > 112 && touch.py < 136)
				drawButtonColored(8,14,14);
			else if (touch.px > 64 && touch.px < 184 && touch.py > 152 && touch.py < 176)
				drawButtonColored(8,19,14);
			else if (gameGen && !multiplayer && !getDied() && touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
				drawButtonColored(25,19,4);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 64 && touch.px < 184 && touch.py > 72 && touch.py < 96)
			{
				gameMode();
				chosen=true;
			}
			else drawButton(8,9,14);
			if (touch.px > 64 && touch.px < 184 && touch.py > 112 && touch.py < 136)
			{
				multiplayerScreen();
				chosen=true;
			}
			else drawButton(8,14,14);
			if (touch.px > 64 && touch.px < 184 && touch.py > 152 && touch.py < 176)
			{
				settings();
				titlescreen_redraw();
			}
			else drawButton(8,19,14);
			if (gameGen && !multiplayer && !getDied() && touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
			{
				drawButtonColored(8,9,14);
				drawBackground();
				consoleClear();
				stopMusic();
				playCalm = false;
				playMusic(HAL2);
				playHal2 = true;
				theWorld = mainGame(1,theWorld);
				chosen=true;
			}
			else if (gameGen && !multiplayer && !getDied())
				drawButton(25,19,4); //Back button
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
	return 0;
}
