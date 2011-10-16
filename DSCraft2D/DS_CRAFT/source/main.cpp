//The main.cpp is the simplist file of all, and it should be
//BUT it depends on many other files that are more complex
#include <nds.h>
#include "ndsvideo.h" // Include the video functions (for setupVideo();)
#include "player.h" // The player fuctions
#include "sprcount.h" //The sprite number counter
#include "world.h" //A World!!!!!
#include <stdio.h> //For Rand
#include <time.h>
#include "blockID.h" //The Block ID numbers to a word
int main(){
	setupVideo(); //Setup all the video we need (in ndsvideo.h/cpp)
	playerActor MainPlayer; //Create a Player Object
	MainPlayer.x=256/2-16;//Place the player in the middle of the screen
	MainPlayer.y=192/2-32;
	worldObject CurrentWorld;
	//CurrentWorld.CamX=256; //Testing stuff
	srand(time(NULL)); //The seed :)
	rand(); //junk
	consoleDemoInit();
	int i,j;
	for(i=0;i<=32;i++){
		for(j=0;j<=32;j++){
			CurrentWorld.blocks[i][j]=rand()%15;//11 is the amount of blocks implemented
	}}
	worldSetUp();
	while(1){
		consoleClear();
		iprintf("%d,%d",CurrentWorld.CamX,CurrentWorld.CamY);
		iprintf(",,%d,%d",MainPlayer.x,MainPlayer.y);
		updateplayer(&MainPlayer,&CurrentWorld);	//Update the player
		worldUpdate(&CurrentWorld);
		swiWaitForVBlank(); //Wait for a VBlank
		oamUpdate(&oamMain); //Update the sprites
		resetSpriteCount(); //And set the sprite number counter to 0

	}
}
