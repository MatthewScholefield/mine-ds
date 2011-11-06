#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
u16* dirtgfx;

int dirtls=0;
void DIRT_render(int x,int y){
	createsprite32x32(x,y,dirtgfx,false,0);	
}
void DIRT_setup(){
	dirtgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*DIRT;
	dmaCopy(blocktiles,dirtgfx,32*32);
}
void DIRT_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (dirtls==0){
				playSound(GRAVEL_A);		
				dirtls++;
			}
			else if (dirtls==1){
				playSound(GRAVEL_B);
				dirtls++;
			}	
			else if (dirtls==2){
				playSound(GRAVEL_C);
				dirtls++;
			}			
			else if (dirtls==3){
				playSound(GRAVEL_D);
				dirtls=0;
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
void DIRT_update(int bx,int by,worldObject* world,playerActor* player){
        int j;
        if (world->data[bx][by]>rand() % 128 + 384){
                for (j=0;j<=WORLD_HEIGHT;j++){
                        if(world->blocks[bx][j]==DIRT && j==by){
                                world->blocks[bx][by]=GRASS;
                                world->data[bx][by]=0;
                                j=WORLD_HEIGHT+1;       
                        }
                        else if (world->blocks[bx][j]!=AIR){ //And If we have not encountered dirt and we are at a different block
                                world->data[bx][by]=0;
                                j=WORLD_HEIGHT+1;                           //Exit this X
                        }
                }

        }
        else world->data[bx][by]++;
}

