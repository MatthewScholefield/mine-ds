#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* GOLD_OREgfx;
void GOLD_ORE_render(int x,int y){
	createsprite32x32(x,y,GOLD_OREgfx,false,0);	
}
void GOLD_ORE_setup(){
	GOLD_OREgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*GOLD_ORE;
	dmaCopy(blocktiles,GOLD_OREgfx,32*32);
}
