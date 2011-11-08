#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
u16* COBBLEgfx;
int cobblels;
void COBBLE_render(int x,int y){
	createsprite32x32(x,y,COBBLEgfx,false,0);	
}
void COBBLE_setup(){
	COBBLEgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*COBBLE;
	dmaCopy(blocktiles,COBBLEgfx,32*32);
}
void COBBLE_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (cobblels==0){
				playSound(STONE_A);		
				cobblels++;
			}
			else if (cobblels==1){
				playSound(STONE_B);
				cobblels++;
			}	
			else if (cobblels==2){
				playSound(STONE_C);
				cobblels++;
			}			
			else if (cobblels==3){
				playSound(STONE_D);
				cobblels=0;
			}
		}	
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
