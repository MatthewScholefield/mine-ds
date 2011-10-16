#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* GRAVELgfx;
void GRAVEL_render(int x,int y){
	createsprite32x32(x,y,GRAVELgfx,false,0);	
}
void GRAVEL_setup(){
	GRAVELgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*GRAVEL;
	dmaCopy(blocktiles,GRAVELgfx,32*32);
}
