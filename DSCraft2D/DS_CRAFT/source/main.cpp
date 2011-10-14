//The main.cpp is the simplist file of all, and it should be
//BUT it depends on many other files that are more complex
#include <nds.h>
#include "ndsvideo.h" // Include the video difanations (for setupVideo();)
#include "player.h" // The player fuctions
#include "sprcount.h" //The sprite number counter
#include "world.h" //A World!!!!!
#include <stdio.h> //For Rand
#include "blockID.h"
int main(){
	setupVideo(); //Setup all the video we need (in ndsvideo.h/cpp)
	playerActor MainPlayer; //Create a Player Object
	MainPlayer.x=256/2-16;//Place the player in the middle of the screen
	MainPlayer.y=192/2-32;
	worldObject CurrentWorld;
	//CurrentWorld.CamX=256; //Testing stuff
	srand(11534355); //The seed :)
	rand(); //junk
	int i;
	for(i=0;i<=32*32;i++){
		CurrentWorld.blocks[i]=rand()%12;
		if (CurrentWorld.blocks[i]==11) CurrentWorld.blocks[i]=8123;//Air
	}
	worldSetUp();
	while(1){
		updateplayer(&MainPlayer,&CurrentWorld);	//Update the player
		worldUpdate(&CurrentWorld);
		swiWaitForVBlank(); //Wait for a VBlank
		oamUpdate(&oamMain); //Update the sprites
		resetSpriteCount(); //And set the sprite number counter to 0

	}
}
