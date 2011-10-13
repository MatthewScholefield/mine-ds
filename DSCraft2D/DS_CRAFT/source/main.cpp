#include <nds.h>
#include "ndsvideo.h"
#include "player.h"
#include "sprcount.h"
int main(){
	setupVideo(); //Setup all the video we need
	playerActor MainPlayer;
	while(1){
		updateplayer(&MainPlayer);	
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		resetSpriteCount();
	}
}
