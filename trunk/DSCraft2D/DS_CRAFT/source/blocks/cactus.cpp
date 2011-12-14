#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* CACTUSgfx;
int CACTUSls;
void CACTUS_render(int x,int y){
	createsprite32x32(x,y,CACTUSgfx,false,0);	
}
void CACTUS_setup(){
	CACTUSgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*CACTUS;
	dmaCopy(blocktiles,CACTUSgfx,32*32);
}
void CACTUS_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		if ((player->person && !(keysHeld() & KEY_DOWN && world->blocks[bx][by-1]==CACTUS)) || player->person==false) playerHurt(player,1,false);	
	}

}
void CACTUS_update(int bx,int by,worldObject* world,playerActor* player){
	world->data[bx][by]+=1;
	if (world->blocks[bx][by+1]==SAND || world->blocks[bx][by+1]==CACTUS);
	else {
		world->blocks[bx][by]=AIR;
		inventoryAdd(CACTUS);	
	}
	
}
void CACTUS_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>36){
		world->blocks[x][y]=AIR;
		inventoryAdd(CACTUS);
		*mine_time=0;
	}
}
