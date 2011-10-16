#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* DARK_WOODgfx;
void DARK_WOOD_render(int x,int y){
	createsprite32x32(x,y,DARK_WOODgfx,false,0);	
}
void DARK_WOOD_setup(){
	DARK_WOODgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*DARK_WOOD;
	dmaCopy(blocktiles,DARK_WOODgfx,32*32);
}
