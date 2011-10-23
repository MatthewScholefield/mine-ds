//The main.cpp is the simplist file of all, and it should be
//BUT it depends on many other files that are more complex
#include <nds.h>
#include "ndsvideo.h" // Include the video functions (for setupVideo();)
#include "player.h" // The player fuctions
#include "sprcount.h" //The sprite number counter
#include "world.h" //A World!!!!!
#include "worldgen.h"
#include <stdio.h> //For Rand
#include <time.h>
#include "mining.h"
#include "blockID.h" //The Block ID numbers to a word
#include "top-screen.h"
#include "day-night.h"
#include "blockupdate.h"
#include "saver.h"
#include "gameover.h"
int main(){
	int framecounte=0; //framecount
	setupVideo(); 
	lcdMainOnBottom();
	scanKeys();
	bool debug = false;
	if (keysDown() & KEY_START || keysHeld() & KEY_START) debug=true;
	playerActor MainPlayer;
	MainPlayer.health=10;
	worldObject* CurrentWorld = (worldObject *) calloc(1, sizeof(worldObject));
	srand(time(NULL));
	if (debug) consoleDemoInit();
	generateWorld(CurrentWorld);
	worldSetUp();
	saveInit();
	CurrentWorld->ChoosedBlock = 255;
	CurrentWorld->DELmode = false;
	doneSetup();
	mainBGSetup();
	if (!debug) subBGSetup();
	//Place the player on the first "non grass" block (The Camera will take one frame to Update)
	int i;
	for (i=0;i<=WORLD_HEIGHT;i++)
		if (CurrentWorld->blocks[0][i]!=AIR)
		{
			MainPlayer.y=i*32-64;
			MainPlayer.x=0;
			i=WORLD_HEIGHT+1;
		}
	while(1){
		framecounte++;
		mainBGUpdate();
		saveUpdate(CurrentWorld,&MainPlayer);
		subLifes(MainPlayer.health);
		subShowBlock(CurrentWorld->ChoosedBlock);
		miningUpdate(CurrentWorld,&MainPlayer);
        if (MainPlayer.health == 0) gameover(CurrentWorld,&MainPlayer);
		if (keysHeld() & KEY_START) playerHurt(&MainPlayer,10,true); //Press start to kill your self :P
		updateplayer(&MainPlayer,CurrentWorld);	//Update the player
		worldUpdate(CurrentWorld);
		updateBlocks(CurrentWorld,&MainPlayer);
		//if (framecounte%240==0) fixgrass(CurrentWorld);
		swiWaitForVBlank(); //Wait for a VBlank
		oamUpdate(&oamMain); //Update the sprites
		oamUpdate(&oamSub);
		consoleClear();
		iprintf("Camera Position:%d,%d\n",CurrentWorld->CamX,CurrentWorld->CamY);
		iprintf("Player Position:%d,%d\n",MainPlayer.x,MainPlayer.y);
		iprintf("Player BlockPos:%d,%d\n",MainPlayer.blockx,MainPlayer.blocky);
		printf("Sprites on Screen: %d\n",nextSprite());
		iprintf("Choosen Block: %d\n",CurrentWorld->ChoosedBlock);
		resetSpriteCount(); //And set the sprite number counter to 0
		if (framecounte>240) framecounte=1;

	}

}
