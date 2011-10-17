#include "world.h"
#include "blockID.h"
#include <stdio.h>
#include <nds.h>	
void fixgrass(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++)
			if (world->blocks[i][j]==GRASS) world->blocks[i][j]=DIRT; //Change all the GRASS blocks into dirt blocks
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if(world->blocks[i][j]==DIRT){
				world->blocks[i][j]=GRASS; //Then if a block should be grass make it grass
				j=54;  			   //And Exit this X
			}
			else if (world->blocks[i][j]!=AIR){ //And If we have not encountered dirt and we are at a different block
				j=54;			    //Exit this X
			}
		}
}
void generateWorld(worldObject* world){
	//This is the function that I dont want to program :P
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++){
			world->blocks[i][j]=DIRT;
		}	
	for (i=0;i<=WORLD_WIDTH;i++)
		world->blocks[i][WORLD_HEIGHT]=BEDROCK;
	for (i=0;i<=WORLD_WIDTH;i++)
		for(j=0;j<=5;j++)
			world->blocks[i][j]=AIR;
	fixgrass(world);
}
void randGenerate(worldObject* world){
	int i,j;
	for(i=0;i<=WORLD_WIDTH;i++){
		for(j=0;j<=WORLD_HEIGHT;j++){
			world->blocks[WORLD_WIDTH][j]=rand()%23;
		}
	}
}
