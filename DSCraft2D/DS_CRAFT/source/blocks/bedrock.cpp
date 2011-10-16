#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* BEDROCKgfx;
void BEDROCK_render(int x,int y){
	createsprite32x32(x,y,BEDROCKgfx,false,0);	
}
void BEDROCK_setup(){
	BEDROCKgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*BEDROCK;
	dmaCopy(blocktiles,BEDROCKgfx,32*32);
}
