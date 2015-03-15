#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* DOOR_OPENgfx;
void DOOR_OPEN_render(int x,int y){
	createsprite32x64(x,y,DOOR_OPENgfx,false,0);	
}
void DOOR_OPEN_setup(){
	DOOR_OPENgfx=oamAllocateGfx(&oamMain,SpriteSize_32x64,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*DOOR_OPEN;
	dmaCopy(blocktiles,DOOR_OPENgfx,32*64);
}
