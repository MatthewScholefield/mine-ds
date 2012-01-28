#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* DOORgfx;
void DOOR_render(int x,int y){
	createsprite32x32(x,y,DOORgfx,false,0);	
}
void DOOR_setup(){
	DOORgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*DOOR;
	dmaCopy(blocktiles,DOORgfx,32*32);
}
void DOOR_update(int bx,int by,worldObject* world,playerActor* player){
	if(world->blocks[bx][by+1]!=AIR) world->blocks[bx][by]=DOOR_CLOSED_BOTTOM;
	else{
		inventoryAdd(DOOR);
		world->blocks[bx][by]=AIR;
	}
}
