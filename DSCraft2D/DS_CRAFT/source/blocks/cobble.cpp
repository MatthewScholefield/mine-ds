#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* COBBLEgfx;
void COBBLE_render(int x,int y){
	createsprite32x32(x,y,COBBLEgfx,false,0);	
}
void COBBLE_setup(){
	COBBLEgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*COBBLE;
	dmaCopy(blocktiles,COBBLEgfx,32*32);
}
