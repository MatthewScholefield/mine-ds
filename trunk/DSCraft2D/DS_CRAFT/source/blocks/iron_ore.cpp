#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* IRON_OREgfx;
void IRON_ORE_render(int x,int y){
	createsprite32x32(x,y,IRON_OREgfx,false,0);	
}
void IRON_ORE_setup(){
	IRON_OREgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*IRON_ORE;
	dmaCopy(blocktiles,IRON_OREgfx,32*32);
}
