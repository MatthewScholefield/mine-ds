#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
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
void BEDROCK_colision(playerActor* player,worldObject* world,int bx,int by,int result){
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
		player->vy=0;	
	}
}
