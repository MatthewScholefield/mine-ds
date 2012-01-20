#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* STICKgfx;
void STICK_render(int x,int y){
	createsprite32x32(x,y,STICKgfx,false,0);	
}
void STICK_setup(){
	STICKgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*STICK;
	dmaCopy(blocktiles,STICKgfx,32*32);
}
