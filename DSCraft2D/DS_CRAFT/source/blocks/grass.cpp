#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* GRASSgfx;
void GRASS_render(int x,int y){
	if (y<192 && x<256 && x>(-32) && y>(-32))createsprite32x32(x,y,GRASSgfx,false,0);	
}
void GRASS_setup(){
	GRASSgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*GRASS;
	dmaCopy(blocktiles,GRASSgfx,32*32);
}
