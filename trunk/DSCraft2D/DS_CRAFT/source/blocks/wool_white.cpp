#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
int hurttime;
u16* WOOL_WHITEgfx;
void WOOL_WHITE_render(int x,int y){
createsprite32x32(x,y,WOOL_WHITEgfx,false,0);
}
void WOOL_WHITE_setup(){
WOOL_WHITEgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
char* blocktiles;
blocktiles=(char*)&blockTiles;
blocktiles+=(32*32)*WOOL_WHITE;
dmaCopy(blocktiles,WOOL_WHITEgfx,32*32);
}
void WOOL_WHITE_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
	}
	if (result==3){//player colides on right
	player->x-=2; //Move him back one so he is not coliding anymore :)
	}
	if (result==1){//colides on left
	player->x+=2;
	}
	if (result==4){
		//colision upwards
		player->vy=0;
		player->y=by*32+33;
	}	
} 