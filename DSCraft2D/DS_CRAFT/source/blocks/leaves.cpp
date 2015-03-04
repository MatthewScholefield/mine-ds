#include <nds.h>
#include "../ndsvideo.h"
#include "block.h"
#include "../blockID.h"
#include "../player.h"
#include "../world.h"
#include "../inventory.h"
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
void LEAVES_colision(playerActor* player,worldObject* world,int bx,int by,int result){
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
void LEAVES_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>18){
		world->blocks[x][y]=AIR;
		inventoryAdd(PLACED_LEAF);
		*mine_time=0;
	}
}
void LEAVES_update(int bx,int by,worldObject* world,playerActor* player){
	if (rand() % 200==0){
		int i,j;
		bool log=false;
		for (i=bx-4;i<=bx+4;i++){
			for(j=by-4;j<=by+4;j++){
				if (world->blocks[i][j]==LOG || world->blocks[i][j]==WHITE_WOOD || world->blocks[i][j]==DARK_WOOD || world->blocks[i][j]==PLACED_LOG || world->blocks[i][j]==PLACED_LOG_W || world->blocks[i][j]==PLACED_LOG_D){
					log=true;
				}
			}
		}
		if (log==false) world->blocks[bx][by]=AIR;
	}
}
