#include "ndsvideo.h"
#include <nds.h>
#include "player.h"
#include "PlayerR.h"
u16* playerGraphics;
void updateplayer(playerActor* player){
	scanKeys();
	if (keysHeld() & KEY_UP) player->y--;
	else if (keysHeld() & KEY_DOWN) player->y++;
	if (keysHeld() & KEY_LEFT) player->x--;
	else if (keysHeld() & KEY_RIGHT) player->x++;
	player->blockx=player->x%8;
	player->blocky=player->y%8;
	createsprite32x64(player->x,player->y,playerGraphics,keysHeld() & KEY_LEFT,1);
}
u16* playerGfx(){
	return playerGraphics;
}
void playerCreateGfx(){
	playerGraphics=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
	dmaCopy(PlayerRTiles,playerGraphics,PlayerRTilesLen);
}
