#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mainGame.h"
#include "multiplayerGame.h"
#include "sounds.h"
#include <nds.h>
#include <stdio.h>
//Single Player/Multiplayer :D
//By the time we reach the title screen, all setup procedures should have been completed!

bool  LRC = false; //LR Controls Boolean
int musicIsOn = 0;

void settings(); //forward statements
int titlescreen();

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
                                drawButtonColored(9,9,12);
                                drawBackground();
				stopMusic();
                                musicIsOn=false;
				multiplayerGame(true);
				chosen=true;
			}
			else drawButton(9,9,12);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{	
                                drawBackground();
				stopMusic();
                                musicIsOn=false;
				multiplayerGame(false);
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
        uint oldKeys;
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
void controls()
{
        uint oldKeys;
	touchPosition touch;
	lcdMainOnTop();
        drawBackground();
	//Draw Buttons
	if (LRC)
	{
		drawButtonColored(9,9,11);
		drawButton(8,14,13);
	}
	else
	{
		drawButton(9,9,11);
		drawButtonColored(8,14,13);		
	}
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
			touchRead(&touch);
			if (touch.px > 200 && touch.px < 240 && touch.py > 152 && touch.py < 176)
                                drawButtonColored(25,19,4);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 72 && touch.px < 176 && touch.py > 72 && touch.py < 96)
			{
                                LRC = true;
				drawButtonColored(9,9,11);
				drawButton(8,14,13);	
			}
			else drawButton(9,9,11);
			if (touch.px > 72 && touch.px < 176 && touch.py > 112 && touch.py < 136)
			{	
                                LRC = false;
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
		}
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
void titlescreen_redraw()
{
	int i,j;
        drawBackground();
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
        drawBackground();
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
				drawButtonColored(8,9,14);
                                drawBackground();
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
				titlescreen_redraw();
			}
			else drawButton(8,19,14);
			
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
	return 0;
}
