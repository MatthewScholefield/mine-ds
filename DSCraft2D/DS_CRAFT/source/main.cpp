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

#include "mob.h"
int main(){
	int framecounte=0; //framecount
	setupVideo(); 
	mobSetup();
	lcdMainOnBottom();
	scanKeys();
	bool debug = true;
	if (keysDown() & KEY_B || keysHeld() & KEY_B) debug=false;
	playerActor MainPlayer;
	MainPlayer.health=10;
	MainPlayer.vy=0;
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
	//Place the player on the first "non grass" block
	int i;
	MainPlayer.x=0;
	MainPlayer.y=0;
	while(1){
		scanKeys();
		mainBGUpdate();
		playerFrame();
		saveUpdate(CurrentWorld,&MainPlayer);
		if (!debug) subLifes(MainPlayer.health);
		if (!debug) subShowBlock(CurrentWorld->ChoosedBlock);
        	if (MainPlayer.health <= 0) gameover(CurrentWorld,&MainPlayer);
		mobUpdate(CurrentWorld,&MainPlayer);
		updateplayer(&MainPlayer,CurrentWorld);	//Update the player
		updateBlocks(CurrentWorld,&MainPlayer);
		renderPlayer(&MainPlayer,CurrentWorld);
		worldUpdate(CurrentWorld,(void*)&MainPlayer);
		swiWaitForVBlank(); //Wait for a VBlank
		oamUpdate(&oamMain); //Update the sprites
		oamUpdate(&oamSub);
		resetSpriteCount(); //And set the sprite number counter to 0
		miningUpdate(CurrentWorld,&MainPlayer);

	}

}
