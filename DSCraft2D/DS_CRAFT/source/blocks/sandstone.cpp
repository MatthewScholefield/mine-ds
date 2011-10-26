#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../player.h"
#include "../world.h"
#include "../sounds.h"
u16* SANDSTONEgfx;

int sandstonelx,sandstonely,sandstonels;
void SANDSTONE_render(int x,int y){
	createsprite32x32(x,y,SANDSTONEgfx,false,0);	
}
void SANDSTONE_setup(){
	SANDSTONEgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*SANDSTONE;
	dmaCopy(blocktiles,SANDSTONEgfx,32*32);
}
void SANDSTONE_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==sandstonelx && by==sandstonely)){
			sandstonelx=bx,sandstonely=by;
			if (sandstonels==0){
				playSound(STONE_A);		
				sandstonels++;
			}
			else if (sandstonels==1){
				playSound(STONE_B);
				sandstonels++;
			}	
			else if (sandstonels==2){
				playSound(STONE_C);
				sandstonels++;
			}			
			else if (sandstonels==3){
				playSound(STONE_D);
				sandstonels=0;
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
		player->y+=2;	
	}
}
