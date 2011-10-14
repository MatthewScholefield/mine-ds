#include <nds.h>
#include "ndsvideo.h" // Include the video difanations (for setupVideo();)
#include "player.h" // The player fuctions
#include "sprcount.h" //The sprite number counter
int main(){
	setupVideo(); //Setup all the video we need (in ndsvideo.h/cpp)
	playerActor MainPlayer; //Create a Player Object
	while(1){
		updateplayer(&MainPlayer);	//Update the player
		swiWaitForVBlank(); //Wait for a VBlank
		oamUpdate(&oamMain); //Update the sprites
		resetSpriteCount(); //And set the sprite number counter to 0
	}
}
