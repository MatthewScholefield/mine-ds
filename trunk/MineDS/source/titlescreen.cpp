#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mainGame.h"
#include "multiplayerGame.h"
#include "sounds.h"
#include <nds.h>
#include <stdio.h>
//Single Player/Multiplayer :D
//By the time we reach the title screen, all setup procedures should have been completed!

bool  LRC = true; //LR Controls Boolean
int musicIsOn = 0;

void settings(); //forward statements
int titlescreen();

void multiplayerScreen()
{
        int i,j;
	uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	for (i=0;i<=32;i++)
		for (j=0;j<=32;j++) setSubBgTile(i,j,0); //Clears the screen by placing black tiles repeatedly  (Is there a command for this?)
	for (i=0;i<=25;i++)
		for (j=0;j<=6;j++)
			setSubBgTile(i+2,j,i+(j*32)); //Draw the MineDS Logo!
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
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
                                drawButtonColored(8,9,12);
			else if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
                                drawButtonColored(8,14,12);
                        else if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
                                drawButton(25,19,4);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
			{
                                drawButtonColored(8,9,14);
				for (i=0;i<=32;i++)
					for (j=7;j<=32;j++) setSubBgTile(i,j,0);
				stopMusic();
                                musicIsOn=false;
				multiplayerGame(true);
				chosen=true;
			}
			else drawButton(8,9,12);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{	
				for (i=0;i<=32;i++)
					for (j=7;j<=32;j++) setSubBgTile(i,j,0);
				stopMusic();
                                musicIsOn=false;
				multiplayerGame(false);
				chosen=true;
			}
			else drawButton(8,14,12);
                        if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
                        {
                          titlescreen();
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
        
        int i,j; //Defining them for later
        uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	for (i=0;i<=32;i++)
		for (j=0;j<=32;j++) setSubBgTile(i,j,0); //Clears screen
        for (i=0;i<=25;i++)
		for (j=0;j<=6;j++)
			setSubBgTile(i+2,j,i+(j*32)); //Draws MineDS Logo!
	drawButton(9,9,12);
	drawButton(9,14,12);
        drawButton(7,19,16);
        drawButton(25,19,4);//Back button
        consoleClear();
        iprintf("\x1b[10;11HMine DS");
	iprintf("\x1b[15;11HCoded by");
        iprintf("\x1b[20;9HCoolAs and Ray");
        iprintf("\x1b[20;26HBack");
	scanKeys();
        bool chosen=false;
        while (!chosen)
        {
            scanKeys();
	if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH)) //New Press
	{
		if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
                        drawButtonColored(8,9,12);
	}
        else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
	{
		if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
		{
                    settings();
                    chosen = true;
		}
		else drawButton(25,19,4);
        }
        	oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
        }
}
void controls()
{
        int i,j; //Defining them for later
        uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	for (i=0;i<=32;i++)
		for (j=0;j<=32;j++) setSubBgTile(i,j,0); //Clears screen
	for (i=0;i<=25;i++)
		for (j=0;j<=6;j++)
			setSubBgTile(i+2,j,i+(j*32)); //Draws MineDS Logo!
	//Draw Buttons
	drawButton(9,9,12);
	drawButton(9,14,12);
        drawButton(25,19,4); //Back button
	consoleClear(); //Removes All text from the screen
	iprintf("\x1b[10;11HWith L R");
	iprintf("\x1b[15;10HWithout L R");
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
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
                                drawButtonColored(8,9,12);
			else if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
                                drawButtonColored(8,14,12);
                        else if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
                                drawButtonColored(8,9,12);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
			{
                                LRC = true;
                                settings();
				chosen=true;
			}
			else drawButton(8,9,12);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{	
                                LRC = false;
                                settings();
				chosen=true;
			}
			else drawButton(8,14,12);
                        if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
                        {
                          settings();
                          chosen = true;
                	}
                	else drawButton(25,19,4);
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}
void settings()
{
        int i,j; //I might as well define them for later
        uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	for (i=0;i<=32;i++)
		for (j=0;j<=32;j++) setSubBgTile(i,j,0); //Clears the screen
	for (i=0;i<=25;i++)
		for (j=0;j<=6;j++)
			setSubBgTile(i+2,j,i+(j*32)); //Draw the MineDS Logo!
	//Draw Buttons
	drawButton(9,9,12);
	drawButton(9,14,12);
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
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
                                drawButtonColored(8,9,12);
			else if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
                                drawButtonColored(8,14,12);
                        else if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
                                drawButtonColored(8,9,12);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
			{
                                controls();
				chosen=true;
			}
			else drawButton(8,9,12);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{
                                credits();
				chosen=true;
			}
			else drawButton(8,14,12);
                        if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
                        {
                                titlescreen();
                                chosen = true;
                	}
                	else drawButton(25,19,4);
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
}
int titlescreen()
{
    
if (musicIsOn != 1)
{
    playMusic(CALM);
    musicIsOn = 1;
}
	uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
	int i,j; // They are famous variables :P
        for (i=0;i<=32;i++)
		for (j=0;j<=32;j++) setSubBgTile(i,j,0); //Clears the screen
	for (i=0;i<=25;i++)
		for (j=0;j<=6;j++)
			setSubBgTile(i+2,j,i+(j*32)); // Show the "MineDS Logo!"
	//Lets start the buttons on line 8!
	drawButton(8,9,14);
	drawButton(8,14,14);
        drawButton(8,19,14);
	//Clear the screen!
	consoleClear();
	//Print the Buttons
	iprintf("\x1b[10;9HSingle Player");
	iprintf("\x1b[15;10HMulti Player");
        iprintf("\x1b[20;12HSettings");
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
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 64 && touch.px < 184 && touch.py > 72 && touch.py < 96)
			{
				//drawButtonColored(8,9,14);
				for (i=0;i<=32;i++)
					for (j=7;j<=32;j++) setSubBgTile(i,j,0);
				consoleClear();
				stopMusic();
                                musicIsOn=0;
				mainGame(0);
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
				chosen=true;
			}
			else drawButton(8,19,14);
			
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
	return 0;
}
