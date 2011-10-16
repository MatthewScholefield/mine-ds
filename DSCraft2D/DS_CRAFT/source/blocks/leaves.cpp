#include <nds.h>
#include "../ndsvideo.h"
#include "block.h"
#include "../blockID.h"
u16* LEAVESgfx;
void LEAVES_render(int x,int y){
	createsprite32x32(x,y,LEAVESgfx,false,0);	
}
void LEAVES_setup(){
	LEAVESgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*LEAVES;
	dmaCopy(blocktiles,LEAVESgfx,32*32);
}
