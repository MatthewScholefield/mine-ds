#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mainGame.h"
#include "multiplayerGame.h"
#include "sounds.h"
#include <nds.h>
#include <stdio.h>
//Single Player/Multiplayer :D
//By the time we reach the title screen, all setup procedures should have been completed!

void multiplayerScreen()
{
	uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	int i,j;
	for (i=0;i<=32;i++)
		for (j=0;j<=32;j++) setSubBgTile(i,j,0);
	for (i=0;i<=25;i++)
		for (j=0;j<=6;j++)
			setSubBgTile(i+2,j,i+(j*32)); //Draw the MineDS Logo!
				
	//Draw Buttons
	drawButton(8,9,12);
	drawButton(8,14,12);
	consoleClear(); //Remove All text from the screen
	iprintf("\x1b[10;9HCreate Game");
	iprintf("\x1b[15;10HJoin Game");	
	scanKeys();
	oldKeys=keysHeld();
	swiWaitForVBlank(); // Get "newKeys"
	bool chosen=false;
	while (!chosen)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
		{
			if (touch.px > 64 && touch.px < 160 && touch.py > 72 && touch.py < 88)
				drawButtonColored(8,9,12);
			else if (touch.px > 64 && touch.px < 160 && touch.py > 112 && touch.py < 128)
				drawButtonColored(8,14,12);	
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 64 && touch.px < 176 && touch.py > 72 && touch.py < 88)
			{
				for (i=0;i<=32;i++)
					for (j=7;j<=32;j++) setSubBgTile(i,j,0);
				stopMusic();
				multiplayerGame(true);
				chosen=true;
			}
			else drawButton(8,9,12);
			if (touch.px > 64 && touch.px < 176 && touch.py > 112 && touch.py < 128)
			{	
				for (i=0;i<=32;i++)
					for (j=7;j<=32;j++) setSubBgTile(i,j,0);
				stopMusic();
				multiplayerGame(false);
				chosen=true;
			}
			else drawButton(8,14,12);
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}
int titlescreen()
{
    playMusic(CALM);
	uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	int i,j; // They are famous variables :P
	for (i=0;i<=25;i++)
		for (j=0;j<=6;j++)
			setSubBgTile(i+2,j,i+(j*32)); // Show the "MineDS Logo!"
	//Lets start the buttons on line 8!
	drawButton(8,9,14);
	drawButton(8,14,14);
	//Clear the screen!
	consoleClear();
	//Print the Buttons
	iprintf("\x1b[10;9HSingle Player");
	iprintf("\x1b[15;10HMulti Player");
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
			if (touch.px > 64 && touch.px < 176 && touch.py > 72 && touch.py < 88)
				drawButtonColored(8,9,14);
			else if (touch.px > 64 && touch.px < 176 && touch.py > 112 && touch.py < 128)
				drawButtonColored(8,14,14);		
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 64 && touch.px < 176 && touch.py > 72 && touch.py < 88)
			{
				drawButtonColored(8,9,14);
				for (i=0;i<=32;i++)
					for (j=7;j<=32;j++) setSubBgTile(i,j,0);
				consoleClear();
				stopMusic();
				mainGame(0);
				chosen=true;
			}
			else drawButton(8,9,14);
			if (touch.px > 64 && touch.px < 176 && touch.py > 112 && touch.py < 128)
			{

				multiplayerScreen();
				stopMusic();
				chosen=true;
			}
			else drawButton(8,14,14);
			
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
	return 0;
}
