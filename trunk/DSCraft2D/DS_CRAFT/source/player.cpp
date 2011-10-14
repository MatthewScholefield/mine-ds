#include "ndsvideo.h" // Include the DS video stuff
#include <nds.h> //Include the DS library (for u16*)
#include "player.h" //Include the player structure
#include "PlayerR.h" //Include the player graphics
#include "world.h" //A world!!!
u16* playerGraphics;
//Update the player
//Called by main();
void updateplayer(playerActor* player,worldObject* world){
	//Scan the keys and move that minecraft guy, soon this will need the world values	
	scanKeys();
	//Move the player
	if (keysHeld() & KEY_UP) player->y--;
	else if (keysHeld() & KEY_DOWN) player->y++;
	if (keysHeld() & KEY_LEFT) player->x--;
	else if (keysHeld() & KEY_RIGHT) player->x++;
	//Testing STUFF in the world.
	if (keysHeld() & KEY_X) world->CamY--;
	else if (keysHeld() & KEY_B ) world->CamY++;
	if (keysHeld() & KEY_Y) world->CamX--;
	else if (keysHeld() & KEY_A ) world->CamX++;
	//Stop at end of map
	if (world->CamX>1024) world->CamX=1024;
	else if (world->CamX<0) world->CamX=0;
	if (world->CamY>1024) world->CamY=1024;
	else if (world->CamY<0) world->CamY=0;
	if (player->x>1024)player->x=1024;
	else if (player->x<0)player->x=0;
	if (player->y>1024) player->y=1024;
	else if (player->y<0) player->y=0;
	//Draw the player on the screen
	if (player->y-world->CamY<192 && player->x-world->CamX<256 && player->x-world->CamX>(-32) && player->y-world->CamY>(-32))createsprite32x64(player->x-world->CamX,player->y-world->CamY,playerGraphics,keysHeld() & KEY_LEFT,1);
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
