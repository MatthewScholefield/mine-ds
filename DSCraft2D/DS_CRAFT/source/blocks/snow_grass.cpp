#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
#include "../sounds.h"
#include "../inventory.h"
u16* SNOW_GRASSgfx;
int SNOW_GRASSls=0;
void SNOW_GRASS_render(int x,int y){
	createsprite32x32(x,y,SNOW_GRASSgfx,false,0);	
}
void SNOW_GRASS_setup(){
	SNOW_GRASSgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*SNOW_GRASS;
	dmaCopy(blocktiles,SNOW_GRASSgfx,32*32);
}
void SNOW_GRASS_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (SNOW_GRASSls==0){
				playSound(GRASS_A);		
				SNOW_GRASSls++;
			}
			else if (SNOW_GRASSls==1){
				playSound(GRASS_B);
				SNOW_GRASSls++;
			}	
			else if (SNOW_GRASSls==2){
				playSound(GRASS_C);
				SNOW_GRASSls++;
			}			
			else if (SNOW_GRASSls==3){
				playSound(GRASS_D);
				SNOW_GRASSls=0;
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
void SNOW_GRASS_update(int bx,int by,worldObject* world,playerActor* player){
	int i=bx;
	int j;
	bool grass=true;
		for (j=0;j<=player->blocky+16 && j<=WORLD_HEIGHT;j++){
			if(world->blocks[i][j]==SNOW_GRASS){ //Then if a block should be grass
				j=WORLD_HEIGHT+1;  			   //Exit this X
			}
			else if (world->blocks[i][j]!=AIR && world->blocks[i][j]!=LEAF && world->blocks[i][j]!=LOG && world->blocks[i][j]!=DARK_WOOD && world->blocks[i][j]!=FLOWER_RED && world->blocks[i][j]!=FLOWER_YELLOW){ 	
				grass=false;
				j=WORLD_HEIGHT+1;			    //Exit this X
			}
		}
	if (grass==false) world->blocks[bx][by]=DIRT;
}
void SNOW_GRASS_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>54 && world->ChoosedBlock!= WOOD_SHOVEL && world->ChoosedBlock!=COBBLE_SHOVEL && world->ChoosedBlock==AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(DIRT);
		*mine_time=0;
	}
	else if(*mine_time>54 && world->ChoosedBlock!= WOOD_SHOVEL && world->ChoosedBlock!=COBBLE_SHOVEL && world->ChoosedBlock!=AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(DIRT);
		*mine_time=0;
		setData(world->ChoosedBlock,2,true); //Add 2 "use" to the asdf
	}
	else if (*mine_time>27 && world->ChoosedBlock==WOOD_SHOVEL){
		world->blocks[x][y]=AIR;
		inventoryAdd(DIRT);
		*mine_time=0;
		setData(WOOD_SHOVEL,1,true); //Add 1 "use"
	}
	else if (*mine_time>15 && world->ChoosedBlock==COBBLE_SHOVEL){
		world->blocks[x][y]=AIR;
		inventoryAdd(DIRT);
		*mine_time=0;
		setData(world->ChoosedBlock,1,true); //Add 1 "use"
	}
}