#include "ndsvideo.h" // Include the DS video stuff
#include <nds.h> //Include the DS library (for u16*)
#include "player.h" //Include the player structure
#include "PlayerR.h" //Include the player graphics
u16* playerGraphics;
//Update the player
//Called by main();
void updateplayer(playerActor* player){
	//Scan the keys and move that minecraft guy, soon this will need the world values	
	scanKeys();
	//Move the player
	if (keysHeld() & KEY_UP) player->y--;
	else if (keysHeld() & KEY_DOWN) player->y++;
	if (keysHeld() & KEY_LEFT) player->x--;
	else if (keysHeld() & KEY_RIGHT) player->x++;
	//Stuff That is Rubbish and I'm probably going to remove.
	player->blockx=player->x%8;
	player->blocky=player->y%8;
	//Draw the player on the screen
	createsprite32x64(player->x,player->y,playerGraphics,keysHeld() & KEY_LEFT,1);
}
//Stuff
u16* playerGfx(){
	//return the player Graphics
	return playerGraphics;
}
void playerCreateGfx(){
	//Copy the player graphics into memory
	//Called by setupVideo();
	playerGraphics=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
	dmaCopy(PlayerRTiles,playerGraphics,PlayerRTilesLen);
}
