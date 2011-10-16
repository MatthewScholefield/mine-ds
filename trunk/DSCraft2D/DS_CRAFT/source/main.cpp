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
	worldObject CurrentWorld;

	srand(time(NULL)); //The seed :)
	consoleDemoInit();
	int i,j;
	for(i=0;i<=33;i++){
		for(j=0;j<=33;j++){
			CurrentWorld.blocks[i][j]=rand()%22;
		}
	}
	CurrentWorld.CamX=0; //Testing stuff
	CurrentWorld.CamY=0;
	MainPlayer.x=CurrentWorld.CamX+128-16;//Place the player in the middle of the screen
	MainPlayer.y=CurrentWorld.CamY+96-32;
	worldSetUp();
	while(1){
		consoleClear();
		updateplayer(&MainPlayer,&CurrentWorld);	//Update the player
		worldUpdate(&CurrentWorld);
		iprintf("Camera Position:%d,%d\n",CurrentWorld.CamX,CurrentWorld.CamY);
		iprintf("Player Position:%d,%d\n",MainPlayer.x,MainPlayer.y);
		iprintf("Player BlockPos:%d,%d\n",MainPlayer.blockx,MainPlayer.blocky);
		iprintf("Sprites on Screen: %d\n",nextSprite());
		swiWaitForVBlank(); //Wait for a VBlank
		oamUpdate(&oamMain); //Update the sprites
		resetSpriteCount(); //And set the sprite number counter to 0

	}
}
