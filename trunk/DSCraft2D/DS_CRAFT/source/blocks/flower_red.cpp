#include <nds.h>
#include "../ndsvideo.h"
#include "block.h"
#include "../player.h"
#include "../world.h"
#include "../blockID.h"
#include "../inventory.h"
u16* FLOWER_REDgfx;
void FLOWER_RED_render(int x,int y){
	createsprite32x32(x,y,FLOWER_REDgfx,false,0);	
}
void FLOWER_RED_setup(){
	FLOWER_REDgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*FLOWER_RED;
	dmaCopy(blocktiles,FLOWER_REDgfx,32*32);
}
void FLOWER_RED_update(int bx,int by,worldObject* world,playerActor* player){
	if (world->blocks[bx][by+1]==AIR) world->blocks[bx][by]=AIR;
	else if (world->blocks[bx][by+1]==FLOWER_RED) world->blocks[bx][by]=AIR;
}
void FLOWER_RED_mine(worldObject* world,int* mine_time,int x,int y){
	inventoryAdd(FLOWER_RED);
	*mine_time=0;
}
