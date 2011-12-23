#include <nds.h>
#include "../ndsvideo.h"
#include "block.h"
#include "../player.h"
#include "../world.h"
#include "../blockID.h"
#include "../inventory.h"
u16* FLOWER_YELLOWgfx;
void FLOWER_YELLOW_render(int x,int y){
	createsprite32x32(x,y,FLOWER_YELLOWgfx,false,0);	
}
void FLOWER_YELLOW_setup(){
	FLOWER_YELLOWgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*FLOWER_YELLOW;
	dmaCopy(blocktiles,FLOWER_YELLOWgfx,32*32);
}
void FLOWER_YELLOW_update(int bx,int by,worldObject* world,playerActor* player){
	if (world->blocks[bx][by+1]==AIR) world->blocks[bx][by]=AIR;
	else if (world->blocks[bx][by+1]==FLOWER_YELLOW) world->blocks[bx][by]=AIR;
}
void FLOWER_YELLOW_mine(worldObject* world,int* mine_time,int x,int y){
	world->blocks[x][y]=AIR;
	inventoryAdd(FLOWER_YELLOW);
	*mine_time=0;
}
