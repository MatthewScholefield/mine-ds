#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
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
		player->y=by*32+33;
	}
}
void DIRT_update(int bx,int by,worldObject* world,playerActor* player){
        int j;
	int i=bx;
        if (world->data[bx][by]>rand() % 256 + 512){
                for (j=0;j<=WORLD_HEIGHT;j++){
                        if(world->blocks[bx][j]==DIRT && j==by){
				bool cangrow;
				int a,b;
				for (a=bx-1;a<=bx+1;a++)
					for(b=by-2;b<=by+1;b++){
						if (world->blocks[a][b]==GRASS) 
                         			       world->blocks[bx][by]=GRASS;	
						else if (world->blocks[a][b]==SNOW_GRASS)	
                         			       world->blocks[bx][by]=SNOW_GRASS;		
					}
                                world->data[bx][by]=0;
                                j=WORLD_HEIGHT+1;       
                        }
                        else if (world->blocks[bx][j]!=AIR && world->blocks[i][j]!=LEAF && world->blocks[i][j]!=LOG && world->blocks[i][j]!=DARK_WOOD && world->blocks[i][j]!=FLOWER_RED && world->blocks[i][j]!=FLOWER_YELLOW){ //And If we have not encountered dirt and we are at a different block
                                world->data[bx][by]=0;
                                j=WORLD_HEIGHT+1;                           //Exit this X
                        }
                }

        }
        else world->data[bx][by]++;
}
void DIRT_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>45 && world->ChoosedBlock!= WOOD_SHOVEL && world->ChoosedBlock==AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(DIRT);
		*mine_time=0;
	}
	else if(*mine_time>45 && world->ChoosedBlock!= WOOD_SHOVEL && world->ChoosedBlock!=AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(DIRT);
		*mine_time=0;
		setData(world->ChoosedBlock,2,true); //Add 1 "use" to the pickaxe...
	}
	else if (*mine_time>24 && world->ChoosedBlock==WOOD_SHOVEL){
		world->blocks[x][y]=AIR;
		inventoryAdd(DIRT);
		*mine_time=0;
		setData(WOOD_SHOVEL,1,true); //Add 1 "use"
	}
}

