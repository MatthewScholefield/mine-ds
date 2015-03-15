#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../player.h"
#include "../world.h"
#include "../sounds.h"
#include "../inventory.h"
u16* GRAVELgfx;
int GRAVELls;
void GRAVEL_render(int x,int y,int i,int j,worldObject* world){
	createsprite32x32(x,y+(world->data[i][j]),GRAVELgfx,false,0);	
}
void GRAVEL_render_nofall(int x,int y){
	createsprite32x32(x,y,GRAVELgfx,false,0);	
}
void GRAVEL_setup(){
	GRAVELgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*GRAVEL;
	dmaCopy(blocktiles,GRAVELgfx,32*32);
}
void GRAVEL_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
		if (!(bx==player->sx && by==player->sy)){
			player->sx=bx,player->sy=by;
			if (GRAVELls==0){
				playSound(GRAVEL_A);		
				GRAVELls++;
			}
			else if (GRAVELls==1){
				playSound(GRAVEL_B);
				GRAVELls++;
			}	
			else if (GRAVELls==2){
				playSound(GRAVEL_C);
				GRAVELls++;
			}			
			else if (GRAVELls==3){
				playSound(GRAVEL_D);
				GRAVELls=0;
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
void GRAVEL_mine(worldObject* world,int* mine_time,int x,int y){
	if (*mine_time>54 && world->ChoosedBlock!= WOOD_SHOVEL && world->ChoosedBlock!=COBBLE_SHOVEL && world->ChoosedBlock==AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(GRAVEL);
		*mine_time=0;
	}
	else if(*mine_time>54 && world->ChoosedBlock!= WOOD_SHOVEL && world->ChoosedBlock!=COBBLE_SHOVEL && world->ChoosedBlock!=AIR){
		world->blocks[x][y]=AIR;
		inventoryAdd(GRAVEL);
		*mine_time=0;
		setData(world->ChoosedBlock,2,true); //Add 1 "use" to the pickaxe...
	}
	else if (*mine_time>27 && world->ChoosedBlock==WOOD_SHOVEL){
		world->blocks[x][y]=AIR;
		inventoryAdd(GRAVEL);
		*mine_time=0;
		setData(WOOD_SHOVEL,1,true); //Add 1 "use"
	}
	else if (*mine_time>15 && world->ChoosedBlock==COBBLE_SHOVEL){
		world->blocks[x][y]=AIR;
		inventoryAdd(GRAVEL);
		*mine_time=0;
		setData(world->ChoosedBlock,1,true); //Add 1 "use"
	}
}
void GRAVEL_update(int bx,int by,worldObject* world,playerActor* player){
	if (world->blocks[bx][by+1]==AIR){
		if (world->data[bx][by]==0) world->data[bx][by]++; //Move the block 1 pixel down...	
		world->data[bx][by]+=world->data[bx][by];
	}
	else world->data[bx][by]=0;
	if (world->data[bx][by]>=32){
		world->data[bx][by]=0;
		world->blocks[bx][by]=AIR;
		world->blocks[bx][by+1]=GRAVEL;
	}
}
