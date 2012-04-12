#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mainGame.h"
#include "sounds.h"
#include <nds.h>
#include <stdio.h>
//Single Player
//By the time we reach the title screen, all setup procedures should have been completed!
void drawButton(int x,int y, int sizex)
{
	int i;
	setSubBgTile(x,y,26);
	setSubBgTile(x,y+2,26,V_FLIP);
	setSubBgTile(x+sizex,y,26,H_FLIP);
	setSubBgTile(x+sizex,y+2,26,BOTH_FLIP);
	for(i=0;i<=13;i++)
		setSubBgTile(x+1+i,y,30);
	for(i=0;i<=13;i++)
		setSubBgTile(x+1+i,y+2,30,V_FLIP);
	setSubBgTile(x,y+1,27);
	setSubBgTile(x+sizex,y+1,27,H_FLIP);
	for (i=0;i<=13;i++)
		setSubBgTile(x+1+i,y+1,28 + (i%2));
}
void drawButtonColored(int x,int y, int sizex)
{
	int i;
	setSubBgTile(x,y,58);
	setSubBgTile(x,y+2,58,V_FLIP);
	setSubBgTile(x+sizex,y,58,H_FLIP);
	setSubBgTile(x+sizex,y+2,58,BOTH_FLIP);
	for(i=0;i<=13;i++)
		setSubBgTile(x+1+i,y,62);
	for(i=0;i<=13;i++)
		setSubBgTile(x+1+i,y+2,62,V_FLIP);
	setSubBgTile(x,y+1,59);
	setSubBgTile(x+sizex,y+1,59,H_FLIP);
	for (i=0;i<=13;i++)
		setSubBgTile(x+1+i,y+1,60 + (i%2));
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
	//Clear the screen!
	consoleClear();
	//Print the Buttons
	iprintf("\x1b[10;9HSingle Player");
	bool chosen=false;
	scanKeys();
	oldKeys=keysHeld();
	swiWaitForVBlank();	
	while (!chosen)
	{
		scanKeys();
		if (keysHeld() & KEY_TOUCH && !(oldKeys & KEY_TOUCH))
		{
			//Check if over SinglePlayerButton
			if (touch.px > 64 && touch.px < 176 && touch.py > 72 && touch.py < 88)
				drawButtonColored(8,9,14);
		}
		else if (!(keysHeld() & KEY_TOUCH) && oldKeys & KEY_TOUCH)
		{
			if (touch.px > 64 && touch.px < 176 && touch.py > 72 && touch.py < 88)
			{
				for (i=0;i<=32;i++)
					for (j=7;j<=32;j++) setSubBgTile(i,j,0);
				consoleClear();
				stopMusic();
				mainGame(0);
			}
			else drawButton(8,9,14);
			
			chosen=true;
		}
		oldKeys=keysHeld();
		touchRead(&touch);
		swiWaitForVBlank();
	}
	return 0;
}
