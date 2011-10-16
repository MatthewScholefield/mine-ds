#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* WHITE_WOODgfx;
void WHITE_WOOD_render(int x,int y){
	createsprite32x32(x,y,WHITE_WOODgfx,false,0);	
}
void WHITE_WOOD_setup(){
	WHITE_WOODgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*WHITE_WOOD;
	dmaCopy(blocktiles,WHITE_WOODgfx,32*32);
}
