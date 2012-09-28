#include <nds.h>
#include "nifi.h"
#include <stdio.h>
#include "People.h"
#include "communications.h"
#include "stage.h"
#include "positions.h"
u16* People[24];

int spriteID;
int aniframe;
bool moving;
int disconnectTimes;
int sendframe;
int breakframe;
int player_x,player_y,player_frame,player_facing;

void recievedMessage()
{
	disconnectTimes=0;
}
int graphicNextMain()
{
	return ++spriteID;
}
void graphicFrame()
{
	spriteID=-1;
	oamClear(&oamMain,0,127);
	oamClear(&oamSub,0,127);
}
#define showGraphic(a,b,c,d) oamSet(&oamMain,graphicNextMain(),b,c,0,0,SpriteSize_32x32,SpriteColorFormat_256Color,a,-1,false,false,d,false,false);  
void renderStage(Stage* a);
void initMainGame()
{

	vramSetBankB(VRAM_B_MAIN_SPRITE);
	oamInit(&oamMain, SpriteMapping_1D_128, false);
	u8* gfx =(u8*) PeopleTiles;
	int i;
	for(i = 0; i < 24; i++)
	{
		People[i] = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
		dmaCopy(gfx, People[i], 32*32);
		gfx += 32*32;
	}
	dmaCopy(PeoplePal, SPRITE_PALETTE, 512);
}
touchPosition touch;
Stage* something;
int nextBlock[]={0, 0, 2, 4, 0, 6, 7, 0}; 
void breakBlock(int x,int y)
{
	something->block[x][y]=nextBlock[something->block[x][y]];
}
void mainGame()
{
	Stage Arena;
	something=&Arena;
	int i,j;
	generateNormalStage(&Arena,0);
	renderStage(&Arena);
	aniframe=0;
	nifiEnable();
	bool chosen=false;
	bool host=true;
	consoleClear();
	sendframe=0;
	iprintf("What do you wish to do?\n");
	iprintf("  Host a Game\n");
	iprintf("  Join a Game\n");
	while(!chosen)
	{
		scanKeys();
		if (host) iprintf("\x1b[1;0H*\n ");
		else iprintf("\x1b[1;0H \n*");
		if (keysDown() & (KEY_UP | KEY_DOWN))
			host = !host;
		if (keysDown() & (KEY_A | KEY_START))
			chosen = true;
	}
	consoleClear();
	if (host)
	{
		while (!hostNifiInit()) swiWaitForVBlank();
		iprintf("Waiting for Clients!\n");
		while (!startGame())
		{
		 swiWaitForVBlank();
		 nifiUpdate();
		}
	}
	if (host==false)
	{
		while (!clientNifiInit()) swiWaitForVBlank();
		iprintf("Connecting...\n");
		if (!doHandshake()) return;
	}
	lcdMainOnBottom();
	consoleClear();
	countdown:
	for (i=5;i>0;i--)
	{
		iprintf("Game Starts in %d Seconds!\n",i); 
		for(j=0;j<=60;j++) swiWaitForVBlank();
	}
	if (isHost())
	{
		player_x = rand() % 256;
		player_y = rand() % 192;
		rand();
		rand();	
	}
	else
	{
		rand();
		rand();	
		player_x = rand() % 256;
		player_y = rand() % 192;
	}
	while (1)
	{
		scanKeys();
		touchRead(&touch);
		
		sendframe++;
		if (sendframe==3)
		{
			sendPosition(player_x,player_y,player_facing,player_frame);
			sendframe=0;
			disconnectTimes++;
		
		}
		if(Arena.block[(player_x+6)/16][player_y/16]==0 && Arena.block[((player_x+6)/16)+1][player_y/16]==0 && Arena.block[(player_x+6)/16][(player_y/16)+1]==0 && Arena.block[((player_x+6)/16)+1][(player_y/16)+1]==0)
		{
			consoleClear();
			sendPosition(player_x,player_y,player_facing,player_frame);
			iprintf("You Lost!\n");
			generateNormalStage(&Arena,rand() % 3);
			goto countdown;
		}
		if(Arena.block[(getbattleX()+6)/16][getbattleY()/16]==0 && Arena.block[((getbattleX()+6)/16)+1][getbattleY()/16]==0 && Arena.block[(getbattleX()+6)/16][(getbattleY()/16)+1]==0 && Arena.block[((getbattleX()+6)/16)+1][(getbattleY()/16)+1]==0)
		{
			consoleClear();
			iprintf("You Won!\n");
			generateNormalStage(&Arena,rand() % 3);
			goto countdown;
		}
		breakframe++;
		if (breakframe>6 && touch.px/16 > player_x/16-2 && touch.px/16 < player_x/16 + 4 && touch.py/16 > player_y/16-2 && touch.py/16 < player_y/16+4)
			{
				breakBlock(touch.px/16,touch.py/16);
				setBlock(touch.px/16,touch.py/16);
				breakframe=0;
			}
		if (disconnectTimes>40) return; //If other player turned of ds (No more communications), die

		if (keysHeld() & KEY_LEFT)
		{
			player_facing = 3;
			player_x-=2;
			moving = true;
		}
		else if (keysHeld() & KEY_RIGHT)
		{
			player_facing = 1;
			player_x+=2;
			moving = true;
		}
		if (keysHeld() & KEY_UP)
		{
			player_facing = 0;
			player_y-=2;
			moving = true;
		}
		else if (keysHeld() & KEY_DOWN)
		{
			player_facing = 2;
			player_y+=2;
			moving = true;
		}
		if (player_x < 11) player_x=11;
		if (player_y < 16) player_y=16;
		if (player_x > (256-42)) player_x = (256-42);
		if (player_y > (192-48)) player_y = (192-48);
		aniframe++;
		
		if (aniframe>6 && moving)
		{
			moving = false;
			aniframe=0;
			player_frame++;
			
		}
		else if (aniframe>6)
		{
			aniframe=0;
		}
		if(player_frame==3) player_frame=0;
		showGraphic(People[getbattleFrame()+(getbattleFacing()*3)+12],getbattleX(),getbattleY(),false);
		showGraphic(People[player_frame+player_facing*3],player_x,player_y,false);
		renderStage(&Arena);
		nifiUpdate();
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		graphicFrame();	
	}
	nifiDisable();	
}
