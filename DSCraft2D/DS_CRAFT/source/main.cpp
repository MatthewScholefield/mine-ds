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
int main(){
	int framecounte=0; //framecount
	setupVideo(); //Setup all the video we need (in ndsvideo.h/cpp)
	lcdMainOnBottom();
	scanKeys();
	bool debug = false;
	if (keysDown() & KEY_START || keysHeld() & KEY_START) debug=true; // Press Start when booting the game to show the debug text :)
	playerActor MainPlayer; //Create a Player Object
	worldObject* CurrentWorld = (worldObject *) calloc(1, sizeof(worldObject));
	srand(time(NULL)); //The seed :)
	if (debug) consoleDemoInit();
	generateWorld(CurrentWorld);
	CurrentWorld->CamX=0; //Testing stuff
	CurrentWorld->CamY=0;
	MainPlayer.x=CurrentWorld->CamX+128-16;//Place the player in the middle of the screen
	MainPlayer.y=CurrentWorld->CamY+96-32;
	worldSetUp();
	CurrentWorld->ChoosedBlock = 255;
	CurrentWorld->DELmode = false;
	doneSetup();
	mainBGSetup(); //Main Background (Yeah we will create later a tiled BG)
	if (!debug) subBGSetup(); //Sub Background, turn this off to see the Debug things...(The white lines ARE the Debugtext xD) Nice, but I want to talk about what needs to go there
	while(1){
		framecounte++;
		mainBGUpdate();
		miningUpdate(CurrentWorld,&MainPlayer);
		updateplayer(&MainPlayer,CurrentWorld);	//Update the player
		worldUpdate(CurrentWorld);
		if (framecounte%240==0) fixgrass(CurrentWorld);
		swiWaitForVBlank(); //Wait for a VBlank
		oamUpdate(&oamMain); //Update the sprites
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
