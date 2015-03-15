#include <nds.h>
#include "../ndsvideo.h"
#include "block.h"
#include "../blockID.h"
#include "../player.h"
#include "../world.h"
u16* GLASSgfx;
void GLASS_render(int x,int y){
	createsprite32x32(x,y,GLASSgfx,false,0);	
}
void GLASS_setup(){
	GLASSgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*GLASS;
	dmaCopy(blocktiles,GLASSgfx,32*32);
}
void GLASS_colision(playerActor* player,worldObject* world,int bx,int by,int result){
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
void GLASS_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>27){
		world->blocks[x][y]=AIR;
		//Can't be picked up...
		*mine_time=0;
	}
}
