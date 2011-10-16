#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* SANDgfx;
void SAND_render(int x,int y){
	createsprite32x32(x,y,SANDgfx,false,0);	
}
void SAND_setup(){
	SANDgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*SAND;
	dmaCopy(blocktiles,SANDgfx,32*32);
}
