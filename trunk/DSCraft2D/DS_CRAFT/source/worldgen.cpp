#include "world.h"
#include "blockID.h"
#include <stdio.h>
#include <nds.h>	
#define COAL_START WORLD_HEIGHT/3+1
#define COAL_RARENESS 48
#define IRON_START 24
#define IRON_RARENESS 64
#define GOLD_START 28
#define GOLD_RARENESS 68
#define REDSTONE_START 35
#define REDSTONE_RARENESS 48
#define DIAMOND_START 38
#define DIAMOND_RARENESS 186
#define LAPIS_START 30
#define LAPIS_RARENESS 88
void fixgrass(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++)
			if (world->blocks[i][j]==GRASS) world->blocks[i][j]=DIRT; //Change all the GRASS blocks into dirt blocks
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if(world->blocks[i][j]==DIRT){
				world->blocks[i][j]=GRASS; //Then if a block should be grass make it grass
				j=WORLD_HEIGHT+1;  			   //And Exit this X
			}
			else if (world->blocks[i][j]!=AIR){ //And If we have not encountered dirt and we are at a different block
				j=WORLD_HEIGHT+1;			    //Exit this X
			}
		}
}
void rockwall(worldObject* world,int x,int y){
	int i;
	for (i=y;i<=WORLD_HEIGHT;i++){
		world->blocks[x][i]=STONE;
	}
}
void addore(worldObject* world){
	int y=COAL_START;
	int i;
	int id=COAL_ORE;
	int rareness=COAL_RARENESS;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
	y=IRON_START;
	id=IRON_ORE;
	rareness=IRON_RARENESS;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
	y=GOLD_START;
	id=GOLD_ORE;
	rareness=GOLD_RARENESS;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
	y=REDSTONE_START;
	id=REDSTONE_ORE;
	rareness=REDSTONE_RARENESS;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
	y=DIAMOND_START;
	id=DIAMOND_ORE;
	rareness=DIAMOND_RARENESS;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
	y=LAPIS_START;
	id=LAPIS_ORE;
	rareness=LAPIS_RARENESS;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%5==0) rareness--;
		y++;
	}
}

void addrock(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if(world->blocks[i][j]==DIRT){
				rockwall(world,i,j+rand()%(3)+3);
				j=WORLD_HEIGHT+1;  			   //And Exit this X
			}
		}
}
void modifyWorld(worldObject* world){

	addrock(world);
	addore(world);
	int x;
	fixgrass(world);
	for (x=0;x<=WORLD_WIDTH;x++){
		world->blocks[x][WORLD_HEIGHT]=BEDROCK;	
	}
}
void generateWorld(worldObject* world){
	int x=0;
	int y=rand() % WORLD_HEIGHT/4 + WORLD_HEIGHT/8;
	int i;
	for(i=0;i<=WORLD_WIDTH;i++)
		for (x=0;x<=WORLD_HEIGHT;x++) world->blocks[i][x]=AIR;
	for (x=0;x<=WORLD_WIDTH;x++){
		y+=rand() % 5-2;
		if (y<5) y-=rand() %3-3;
		if (y>WORLD_HEIGHT/3) y-=rand()%3;
		int i;
		printf("%d\n",y);
		for (i=y;i<=WORLD_HEIGHT;i++) world->blocks[x][i]=DIRT;
	}
	modifyWorld(world);
}
void randGenerate(worldObject* world){
	int i,j;
	for(i=0;i<=WORLD_WIDTH;i++){
		for(j=0;j<=WORLD_HEIGHT;j++){
			world->blocks[WORLD_WIDTH][j]=rand()%23;
		}
	}
}
