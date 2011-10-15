#include "world.h"
#include "blockID.h"
#include "blocks/stone.h"
#include "blocks/grass.h"
#include "blocks/dirt.h"
int i,j;
void worldUpdate(worldObject* world){
	for (i=0;i<=32;i++){//Cycle through the block array
		for (j=0;j<=32;j++){
			if (world->blocks[i][j]==STONE) STONE_render(i*32-world->CamX,j*32-world->CamY);	
			else if (world->blocks[i][j]==GRASS) GRASS_render(i*32-world->CamX,j*32-world->CamY);	
			else if (world->blocks[i][j]==DIRT) DIRT_render(i*32-world->CamX,j*32-world->CamY);	
		}	
	}
}
void worldSetUp(){
	STONE_setup();
	GRASS_setup();
	DIRT_setup();
}
