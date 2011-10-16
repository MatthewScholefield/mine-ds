#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* DIAMOND_OREgfx;
void DIAMOND_ORE_render(int x,int y){
	createsprite32x32(x,y,DIAMOND_OREgfx,false,0);	
}
void DIAMOND_ORE_setup(){
	DIAMOND_OREgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*DIAMOND_ORE;
	dmaCopy(blocktiles,DIAMOND_OREgfx,32*32);
}
