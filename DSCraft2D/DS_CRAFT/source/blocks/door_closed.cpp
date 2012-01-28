#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../sounds.h"
#include "../world.h"
#include "../player.h"
#include "../inventory.h"
u16* DOOR_CLOSEDgfx;
int DOORls;
void DOOR_CLOSED_render(int x,int y){
	createsprite32x64(x,y,DOOR_CLOSEDgfx,false,0);	
}
void DOOR_CLOSED_setup(){
	DOOR_CLOSEDgfx=oamAllocateGfx(&oamMain,SpriteSize_32x64,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*DOOR_CLOSED;
	dmaCopy(blocktiles,DOOR_CLOSEDgfx,32*64);
	DOORls=0;
}
void DOOR_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (DOORls==0){
				playSound(WOOD_A);		
				DOORls++;
			}
			else if (DOORls==1){
				playSound(WOOD_B);
				DOORls++;
			}	
			else if (DOORls==2){
				playSound(WOOD_C);
				DOORls++;
			}			
			else if (DOORls==3){
				playSound(WOOD_D);
				DOORls=0;
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
void ACTUAL_DOORUpdate(worldObject* world,int x,int y){
	if(world->blocks[x][y]==DOOR_CLOSED_BOTTOM) world->blocks[x][y-1]=DOOR_CLOSED_TOP;
	else if (world->blocks[x][y]==DOOR_OPEN_BOTTOM) world->blocks[x][y-1]=DOOR_OPEN_TOP;
	if(world->blocks[x][y+1]==AIR)
	{
		world->blocks[x][y]=AIR;
		world->blocks[x][y-1]=AIR;
		inventoryAdd(DOOR);
	}
}
void DOOR_TOP_update(worldObject* world,int x,int y){
	if(world->blocks[x][y+1]==AIR) world->blocks[x][y]=AIR;
}
void DOOR_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>270 && world->ChoosedBlock!= WOOD_AXE && world->ChoosedBlock==AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(DOOR);
		*mine_time=0;
	}
	else if(*mine_time>270 && world->ChoosedBlock!= WOOD_AXE && world->ChoosedBlock!=AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(DOOR);
		*mine_time=0;
		setData(world->ChoosedBlock,1,true); //Add 1 "use" to the asdf
	}

}
