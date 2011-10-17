#include "ndsvideo.h" // Include the DS video stuff
#include <stdio.h>
#include <nds.h> //Include the DS library (for u16*)
#include "player.h" //Include the player structure
#include "PlayerR.h" //Include the player graphics
#include "world.h" //A world!!!
#include "allblocks.h"
#include "gameshelper.h"
#include "blockID.h"
#define INSIDE 0
#define LEFT 1
#define UNDER 2
#define RIGHT 3
#define UP 4
#define U_L 5
#define U_R 6
u16* playerGraphics;
#define gravity 1
int colisionAdv(int blockx1,int blocky1,int blockx2,int blocky2,int x1,int y1,int x2,int y2){
	if (spritecol(x1+8,y1,x2,y2,16,64,32,32)){
		//A good square colision
			if (blockx1==blockx2 && blocky1 == blocky2){
				iprintf("Inside block\n");
				return INSIDE;
			}
			if (blockx1==blockx2 && blocky1+1 == blocky2){
				iprintf("Top of block\n");
				return UNDER;
			}
			if (blockx1+1==blockx2 && blocky1 == blocky2){
				iprintf("block on right\n");
				return RIGHT;
			}
			if (blockx1-1==blockx2 && blocky1 == blocky2){
				iprintf("block on left\n");
				return LEFT;
			}
			if (blockx1==blockx2 && blocky1-1 == blocky2){
				iprintf("block above\n");
				return UP;
			}
	}	
	return 7;
}
void playerGravity(playerActor* player,worldObject* world){
	//colisionAdv(player->blockx,player->blocky,i,j,player->x,player->y,i*32,j*32)
}
//Update the player
//Called by main();
void updateplayer(playerActor* player,worldObject* world){
	//Scan the keys and move that minecraft guy, soon this will need the world values	
	scanKeys();
	//Move the player
	if (keysHeld() & KEY_UP) player->y--;
	else if (keysHeld() & KEY_DOWN) player->y++;
	if (keysHeld() & KEY_LEFT)player->x--;
	else if (keysHeld() & KEY_RIGHT) player->x++;
	//Testing STUFF in the world.
	if (keysHeld() & KEY_UP) world->CamY--;
	else if (keysHeld() & KEY_DOWN ) world->CamY++;
	if (keysHeld() & KEY_LEFT) world->CamX--;
	else if (keysHeld() & KEY_RIGHT ) world->CamX++;
	//Create the block positions
	player->blockx=(player->x+15)/32;
	player->blocky=(player->y+32)/32;	
	//Stop at end of map
	if (world->CamX>world_widthpx-(256-32)) world->CamX=world_widthpx-(256-32);
	else if (world->CamX<0) world->CamX=0;
	if (world->CamY>world_heightpx-(192-32)) world->CamY=world_heightpx-(192-32); //Why the take 32? try it without to find out, (it hides the last block)
	else if (world->CamY<0) world->CamY=0;
	if (player->x>world_widthpx) player->x=world_widthpx;
	else if (player->x<0)player->x=0;
	if (player->y>world_heightpx-32) player->y=world_heightpx-32;//Take one block (even though the last block is bedrock :P)
	else if (player->y<0) player->y=0;
	//Draw the player on the screen
	createsprite32x64(player->x-world->CamX,player->y-world->CamY,playerGraphics,keysHeld() & KEY_LEFT,1);
	int i,j;
	/*for (i=0;i<=WORLD_WIDTH;i++)
		for(j=0;j<=WORLD_HEIGHT;j++)
		{
			if (world->blocks[i][j]!=AIR) colisionAdv(player->blockx,player->blocky,i,j,player->x,player->y,i*32,j*32);
		}*/
	playerGravity(player,world);
}
//Stuff
u16* playerGfx(){
	//return the player Graphics
	return playerGraphics;
}
void playerCreateGfx(){
	//Copy the player graphics into memory
	//Called by setupVideo();
	playerGraphics=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
	dmaCopy(PlayerRTiles,playerGraphics,PlayerRTilesLen);
}
