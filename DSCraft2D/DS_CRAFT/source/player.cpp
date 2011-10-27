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
#include "PlayerHurt.h"
#include "PlayerHit.h"
#include "sounds.h"
#define INSIDE 0
#define LEFT 1
#define UNDER 2
#define RIGHT 3
#define UP 4
#define U_L 5
#define U_R 6
int framecount;
u16* playerGraphics;
bool top;
bool oldtop;
#define gravity 1
int spritecol(int fx,int fy,int sx,int sy,int fSizex,int fSizey,int sSizex,int sSizey){
	if ((fx + fSizex > sx )&& (fx < sx+sSizex) && (sy + sSizey > fy) && (sy < fy+fSizey)) 
		return 1;
	else 
		return 0;
	return 0;
}
int colisionAdv(int blockx1,int blocky1,int blockx2,int blocky2,int x1,int y1,int x2,int y2){
	if (spritecol(x1+11,y1,x2,y2,10,64,32,32)){
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
			if (blockx1+1==blockx2 && blocky1+1 == blocky2 && !oldtop){
				return RIGHT;
			}
			if (blockx1-1==blockx2 && blocky1+1 == blocky2 && !oldtop){
				return LEFT;
			}
			if (blockx1-1==blockx2 && blocky1 == blocky2){
				return LEFT;
			}
			if (blockx1-1==blockx2 && blocky1-1 == blocky2){
				return LEFT;
			}
			if (blockx1==blockx2 && blocky1-1 == blocky2){
				return UP;
			}
	}	
	return 7;
}
void playerGravity(playerActor* player,worldObject* world){
	//colisionAdv(player->blockx,player->blocky,i,j,player->x,player->y,i*32,j*32)
	//printf("Gravity\n");
	int x,y;
	oldtop=player->onblock;
	player->onblock=false;
	if (player->vy<-12) player->vy=-12; //Make the player not go too fast upwards.
	if (player->vy<12 && framecount %4==0) player->vy+=gravity; //vy is speed
	for (x=player->blockx-16;x<=player->blockx+16 && x<=WORLD_WIDTH;x++)//Cycle through the block array
		for (y=player->blocky-16;y<=player->blocky+16 && y<=WORLD_HEIGHT;y++){
			if (x<=-1) x=-1;
			if (y<=-1) y=-1;
			int result=colisionAdv(player->blockx,player->blocky,x,y,player->x,player->y,x*32,y*32);
			//This handles collisions when adding a block copy a line and change the *******_colision to yourblock_colision :P
			if (world->blocks[x][y]==GRASS) GRASS_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==DIRT) DIRT_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==BEDROCK) BEDROCK_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==COBBLE) COBBLE_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==COAL_ORE) COAL_ORE_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==DIAMOND_ORE) DIAMOND_ORE_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==GOLD_ORE) GOLD_ORE_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==GRAVEL) GRAVEL_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==IRON_ORE) IRON_ORE_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==REDSTONE_ORE) REDSTONE_ORE_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==PLANKS) PLANKS_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==PLACED_LOG) LOG_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==SAND) SAND_colision(player,world,x,y,result);			
			else if (world->blocks[x][y]==SANDSTONE) SANDSTONE_colision(player,world,x,y,result);			
			else if (world->blocks[x][y]==STONE) STONE_colision(player,world,x,y,result);			
			else if (world->blocks[x][y]==PLACED_LOG_W) WHITE_WOOD_colision(player,world,x,y,result);			
			else if (world->blocks[x][y]==PLACED_LOG_D) DARK_WOOD_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==LAPIS_ORE) LAPIS_ORE_colision(player,world,x,y,result);
			else if (world->blocks[x][y]==GLASS) GLASS_colision(player,world,x,y,result);
			//Here Lapis-BLOCK
			else if (world->blocks[x][y]==WOOL_WHITE) WOOL_WHITE_colision(player,world,x,y,result); 
			else if (world->blocks[x][y]==PLACED_LEAF) LEAVES_colision(player,world,x,y,result);
			
		}
	if(player->onblock==false) player->y+=player->vy;
	if((keysHeld() & KEY_A || keysHeld() & KEY_UP) && player->onblock==1)
	{
		player->vy=-5; // The -value is the rate which the guy jumps (DONT make it 1 hundred :P OR 1)
		player->y-=1; //Make it come off the ground (not collide)
	}
	framecount++;
}
void updateplayer(playerActor* player,worldObject* world){
	//Scan the keys and move that minecraft guy, soon this will need the world values	
	scanKeys();
	if (keysHeld() & KEY_LEFT)player->x-=2;
	else if (keysHeld() & KEY_RIGHT) player->x+=2;
	world->CamX=player->x-(256/2-16);
	world->CamY=player->y-(192/2-32);
	//Create the block positions
	player->blockx=(player->x+15)/32;
	player->blocky=(player->y+32)/32;	
	//Stop at end of map
	if (world->CamX>world_widthpx-(256-32)) world->CamX=world_widthpx-(256-32);
	else if (world->CamX<0) world->CamX=0;
	if (world->CamY>world_heightpx-(192-32)) world->CamY=world_heightpx-(192-32);
	else if (world->CamY<0) world->CamY=0;
	if (player->x>world_widthpx) player->x=world_widthpx;
	else if (player->x<0)player->x=0;
	if (player->y>world_heightpx-32) player->y=world_heightpx-32;
	else if (player->y<0) player->y=0;
	playerGravity(player,world);
	createsprite32x64(player->x-world->CamX,player->y-world->CamY,playerGraphics,keysHeld() & KEY_LEFT,1);
}
u16* playerGfx(){
	return playerGraphics;
}
void playerCreateGfx(){
	//Copy the player graphics into memory
	//Called by setupVideo();
	playerGraphics=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
	dmaCopy(PlayerRTiles,playerGraphics,PlayerRTilesLen);
}
void test(){
	playerGraphics=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
    dmaCopy(PlayerHitTiles,playerGraphics,PlayerHitTilesLen);

}
void playerHurt(playerActor* player,int much,bool instant){
	
	if (framecount%60==0 || instant){
		player->health-=much;
		playSound(HURT);
	}
}

