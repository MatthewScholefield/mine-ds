#include "world.h"
#include "blockID.h"
#include <stdio.h>
#include <nds.h>

void generateWorld(worldObject* world){
	//This is the function that I dont want to program :P
	int i,j;
	for (i=0;i<=33;i++)
		for (j=0;j<=33;j++){
			world->blocks[i][j]=DIRT;
		}	
	for (i=0;i<=33;i++)
		world->blocks[i][31]=BEDROCK;
}
void randGenerate(worldObject* world){
	int i,j;
	for(i=0;i<=33;i++){
		for(j=0;j<=33;j++){
			world->blocks[i][j]=rand()%23;
		}
	}
}
