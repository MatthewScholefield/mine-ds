#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* LOGgfx;
void LOG_render(int x,int y){
	createsprite32x32(x,y,LOGgfx,false,0);	
}
void LOG_setup(){
	LOGgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*LOG;
	dmaCopy(blocktiles,LOGgfx,32*32);
}
