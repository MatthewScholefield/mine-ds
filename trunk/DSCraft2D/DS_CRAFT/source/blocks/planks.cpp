#include <nds.h>
#include "../ndsvideo.h"
#include "block.h"
#include "../blockID.h"
u16* PLANKSgfx;
void PLANKS_render(int x,int y){
	createsprite32x32(x,y,PLANKSgfx,false,0);	
}
void PLANKS_setup(){
	PLANKSgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*PLANKS;
	dmaCopy(blocktiles,PLANKSgfx,32*32);
}
