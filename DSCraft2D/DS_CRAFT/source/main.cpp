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
#include "blockID.h" //The Block ID numbers to a word

int main(){
	int framecounte=0; //framecount
	setupVideo(); //Setup all the video we need (in ndsvideo.h/cpp)
	lcdMainOnBottom();
	playerActor MainPlayer; //Create a Player Object
	worldObject CurrentWorld;
	srand(time(NULL)); //The seed :)
	consoleDemoInit();
	generateWorld(&CurrentWorld);
	CurrentWorld.CamX=0; //Testing stuff
	CurrentWorld.CamY=0;
	MainPlayer.x=CurrentWorld.CamX+128-16;//Place the player in the middle of the screen
	MainPlayer.y=CurrentWorld.CamY+96-32;
	worldSetUp();
	touchPosition touch;
	CurrentWorld.ChoosedBlock = 0;
	CurrentWorld.DELmode = false;

	while(1){
		framecounte++;
		scanKeys();
		if (keysDown() & KEY_L && CurrentWorld.DELmode == false){ //Switchting between blocks
		    CurrentWorld.ChoosedBlock-=1; //One block down
			}
		if (keysDown() & KEY_R && CurrentWorld.DELmode == false){
		    CurrentWorld.ChoosedBlock+=1; //One block up
			}
			
		if (keysDown() & KEY_SELECT){
		    if (CurrentWorld.DELmode == false){
		          CurrentWorld.DELmode = true;
		          CurrentWorld.ChoosedBlock = 255; //AIR
				  }
			else if (CurrentWorld.DELmode == true){
			      CurrentWorld.DELmode = false;
				  CurrentWorld.ChoosedBlock = 0;
				  }
			}
			
		if (keysHeld() & KEY_TOUCH){
			touchRead(&touch);
			int lax=touch.px/32;
			int lay=touch.py/32;
			lax+=CurrentWorld.CamX/32;
			lay+=CurrentWorld.CamY/32;
        		CurrentWorld.blocks[lax][lay]=CurrentWorld.ChoosedBlock; //WorldObject has now ChooseNBlock //Hrm I'd prefer not, can we add the mining stuff to a mining.cpp file and keep the Choosenblock in that .cpp file) 

		}
		updateplayer(&MainPlayer,&CurrentWorld);	//Update the player
		worldUpdate(&CurrentWorld);
		if (framecounte%240==0) fixgrass(&CurrentWorld);
		swiWaitForVBlank(); //Wait for a VBlank
		oamUpdate(&oamMain); //Update the sprites
		consoleClear();
		iprintf("Camera Position:%d,%d\n",CurrentWorld.CamX,CurrentWorld.CamY);
		iprintf("Player Position:%d,%d\n",MainPlayer.x,MainPlayer.y);
		iprintf("Player BlockPos:%d,%d\n",MainPlayer.blockx,MainPlayer.blocky);
		iprintf("Sprites on Screen: %d\n",nextSprite());
		iprintf("Choosen Block: %d\n",CurrentWorld.ChoosedBlock);
		resetSpriteCount(); //And set the sprite number counter to 0
		if (framecounte>240) framecounte=1;

	}
}
