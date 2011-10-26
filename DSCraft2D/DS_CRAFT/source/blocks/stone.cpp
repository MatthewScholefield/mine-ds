#include <nds.h> //BLAH
#include "../player.h"
#include "../world.h"
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../sounds.h"
u16* stonegfx;
int stonelx,stonely,stonels;
void STONE_render(int x,int y){
	createsprite32x32(x,y,stonegfx,false,0);	
}
void STONE_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==stonelx && by==stonely)){
			stonelx=bx,stonely=by;
			if (stonels==0){
				playSound(STONE_A);		
				stonels++;
			}
			else if (stonels==1){
				playSound(STONE_B);
				stonels++;
			}	
			else if (stonels==2){
				playSound(STONE_C);
				stonels++;
			}			
			else if (stonels==3){
				playSound(STONE_D);
				stonels=0;
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
void STONE_setup(){
	stonegfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*STONE;
	dmaCopy(blocktiles,stonegfx,32*32);
	stonels=0;
}
