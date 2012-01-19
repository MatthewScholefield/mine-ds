#include "ndsvideo.h" // Include the DS video stuff
#include <stdio.h>
#include <nds.h> //Include the DS library (for u16*)
#include "player.h" //Include the player structure
#include "PlayerR.h" //Include the player graphics
#include "world.h" //A world!!!
#include "allblocks.h"
#include "gameshelper.h"
#include "blockID.h"
#include "top-screen.h"
#include "sounds.h"
#include "colision.h"
#define INSIDE 0
#define LEFT 1
#define UNDER 2
#define RIGHT 3
#define UP 4
#define U_L 5
#define U_R 6
int framecount;
u16* playerGraphics[3];
bool top;
#define gravity 1

int colisionAdv(int blockx1,int blocky1,int blockx2,int blocky2,int x1,int y1,int x2,int y2,playerActor* player){
	if (spritecol(x1+11,y1+6,x2,y2,10,58,32,32)){
		//A good square colision
			if (blockx1==blockx2 && blocky1 == blocky2){
				return INSIDE;
			}
			if (blockx1==blockx2 && blocky1+1 == blocky2){
				top=true;
				return UNDER;
			}
			if (blockx1+1==blockx2 && blocky1 == blocky2){
				return RIGHT;
			}
			if (blockx1+1==blockx2 && blocky1-1 == blocky2){
				return RIGHT;
			}
			if (blockx1+1==blockx2 && blocky1+1 == blocky2 && !player->oldtop){
				return RIGHT;
			}
			if (blockx1-1==blockx2 && blocky1+1 == blocky2 && !player->oldtop){
				return LEFT;
			}
			if (blockx1-1==blockx2 && blocky1 == blocky2){
				return LEFT;
			}
			if (blockx1-1==blockx2 && blocky1-1 == blocky2){
				return LEFT;
			}
			if (blockx1==blockx2 && blocky1-(player->height-1) == blocky2){
				return UP;
			}
	}	
	return 7;
}
void playerGravity(playerActor* player,worldObject* world){
	int x,y;
	player->oldtop=player->onblock;
	player->onblock=false;
	if (player->vy<-12) player->vy=-12; //Make the player not go too fast upwards.
	if (player->vy<12 && framecount %4==0) player->vy+=gravity; //vy is speed
	for (x=player->blockx-2;x<=player->blockx+2 && x<=WORLD_WIDTH;x++)//Cycle through the block array
		for (y=player->blocky-2;y<=player->blocky+2 && y<=WORLD_HEIGHT;y++){
			if (x<=-1) x=-1;
			if (y<=-1) y=-1;
			int result=colisionAdv(player->blockx,player->blocky,x,y,player->x,player->y,x*32,y*32,player);
			//This handles collisions when adding a block copy a line and change the *******_colision to yourblock_colision :P
			if (world->blocks[x][y]!=AIR || world->blocks[x][y]!=LEAF || world->blocks[x][y]!=LOG){
				switch (world->blocks[x][y])
				{
				case GRASS: GRASS_colision(player,world,x,y,result); break;
				case SNOW_GRASS: SNOW_GRASS_colision(player,world,x,y,result); break;
				case DIRT:  DIRT_colision(player,world,x,y,result);  break;
				case STONE: STONE_colision(player,world,x,y,result); break;	
				case PLACED_LOG_W: WHITE_WOOD_colision(player,world,x,y,result);	break;		
				case PLACED_LOG_D: DARK_WOOD_colision(player,world,x,y,result); break;	
				case PLACED_LEAF: LEAVES_colision(player,world,x,y,result); break;	
				case PLANKS: PLANKS_colision(player,world,x,y,result); break;
				case PLACED_LOG: LOG_colision(player,world,x,y,result); break;
				case COBBLE: COBBLE_colision(player,world,x,y,result); break;
				case COAL_ORE: COAL_ORE_colision(player,world,x,y,result); break;
				case DIAMOND_ORE: DIAMOND_ORE_colision(player,world,x,y,result); break;
				case GOLD_ORE: GOLD_ORE_colision(player,world,x,y,result); break;
				case GRAVEL: GRAVEL_colision(player,world,x,y,result); break;
				case IRON_ORE: IRON_ORE_colision(player,world,x,y,result); break;
				case REDSTONE_ORE: REDSTONE_ORE_colision(player,world,x,y,result); break;
				case SAND: SAND_colision(player,world,x,y,result); break;
				case SANDSTONE: SANDSTONE_colision(player,world,x,y,result);	 break;
				case LAPIS_ORE: LAPIS_ORE_colision(player,world,x,y,result); break;
				case GLASS: GLASS_colision(player,world,x,y,result); break;
				case BEDROCK: BEDROCK_colision(player,world,x,y,result); break;
				case CACTUS: CACTUS_colision(player,world,x,y,result); break;
				//Here Lapis-BLOCK 
				case WOOL_WHITE: WOOL_WHITE_colision(player,world,x,y,result);  break;
				}
			}
			
		}
	if(player->onblock==false) player->y+=player->vy;
	if((keysHeld() & (KEY_UP | KEY_A)) && player->onblock==1 && player->person)
	{
		player->vy=-5; // The -value is the rate which the guy jumps (DONT make it 1 hundred :P OR 1)
		player->y-=1; //Make it come off the ground (not collide)
	}
}
void updateplayer(playerActor* player,worldObject* world){
	//[s]Scan the keys and move that minecraft guy, soon this will need the world values[/s] LOLOLOL xD	
	if (player->framecount!=0) player->framecount--;
	if (keysHeld() & KEY_LEFT && player->person){
		player->x-=2;
		player->facing_left=true;
	}
	else if (keysHeld() & KEY_RIGHT && player->person){
		player->x+=2;
		player->facing_left=false;
	}
	if (player->frame==1 && player->facing_left && player->framecount%2) player->x++;
	else if (player->frame==1 && player->facing_left==false && player->framecount%2) player->x--;
	if (player->person) world->CamX=player->x-(256/2-16);
	if (player->person) world->CamY=player->y-(192/2-32);
	//Createthe block positions
	player->blockx=(player->x+15)/32;
	player->blocky=(player->y+32)/32;	
	//Stop at end of map
	if (player->person){
		if (world->CamX>world_widthpx-(256-32)) world->CamX=world_widthpx-(256-32);
		else if (world->CamX<0) world->CamX=0;
		if (world->CamY>world_heightpx-(192-32)) world->CamY=world_heightpx-(192-32);
		else if (world->CamY<0) world->CamY=0;
	}
	if (player->x>world_widthpx) player->x=world_widthpx;
	else if (player->x<0)player->x=0;
	if (player->y>world_heightpx-32) player->y=world_heightpx-32;
	else if (player->y<0) player->y=0;
	playerGravity(player,world);
}
void renderPlayer(playerActor* player,worldObject* world){
	if (player->frame!=0) player->frametime--;
	if (player->frametime==0) player->frame=0;
	if (!(player->frame==2 && player->frametime>7)) createsprite32x64(player->x-world->CamX,player->y-world->CamY,playerGraphics[player->frame],player->facing_left,1); 
	else createsprite32x64(player->x-world->CamX,player->y-world->CamY,playerGraphics[0],player->facing_left,1); 
}
u16* playerGfx(){
	return playerGraphics[0];
}
void playerCreateGfx(){
	//Copy the player graphics into memory
	//Called by setupVideo();
	playerGraphics[0]=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
	dmaCopy(PlayerRTiles,playerGraphics[0],32*64);
	u8* Tiles=(u8*)&PlayerRTiles;
	Tiles+=32*64;
	playerGraphics[1]=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
	dmaCopy(Tiles,playerGraphics[1],32*64);	
	Tiles+=32*64;
	playerGraphics[2]=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
	dmaCopy(Tiles,playerGraphics[2],32*64);
	
}
void PlayerPunch(playerActor* player){
	if (player->frame==0){
		player->frame=2;
		player->frametime=10;
	}
}
void playerFrame(){
	framecount++;
}


void playerHurt(playerActor* player,int much,bool instant){
	
	if (player->framecount==0 || instant){
		player->health-=much;
		playSound(HURT);
		player->vy=-4;
		player->y-=1;
		player->frame=1;
		player->frametime=30;
		player->framecount=60;
	}
}

