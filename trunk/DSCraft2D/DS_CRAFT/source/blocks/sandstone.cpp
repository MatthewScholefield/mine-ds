#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* SANDSTONEgfx;
void SANDSTONE_render(int x,int y){
	createsprite32x32(x,y,SANDSTONEgfx,false,0);	
}
void SANDSTONE_setup(){
	SANDSTONEgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*SANDSTONE;
	dmaCopy(blocktiles,SANDSTONEgfx,32*32);
}
