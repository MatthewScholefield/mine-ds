#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* COAL_OREgfx;
void COAL_ORE_render(int x,int y){
	createsprite32x32(x,y,COAL_OREgfx,false,0);	
}
void COAL_ORE_setup(){
	COAL_OREgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*COAL_ORE;
	dmaCopy(blocktiles,COAL_OREgfx,32*32);
}
