#include "world.h"
#include "blockID.h"
#include "allblocks.h"

int i,j;
void worldUpdate(worldObject* world){
	for (i=0;i<=32;i++){//Cycle through the block array
		for (j=0;j<=32;j++){
			if (world->blocks[i][j]==STONE) STONE_render(i*32-world->CamX,j*32-world->CamY);	
			else if (world->blocks[i][j]==GRASS) GRASS_render(i*32-world->CamX,j*32-world->CamY);	
			else if (world->blocks[i][j]==DIRT) DIRT_render(i*32-world->CamX,j*32-world->CamY);	
			else if (world->blocks[i][j]==LOG) LOG_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==LEAVES) LEAVES_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==PLANKS) PLANKS_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==TORCH) TORCH_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==COBBLE) COBBLE_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==BEDROCK) BEDROCK_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==SAND) SAND_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==DOOR_OPEN_TOP) DOOR_OPEN_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==DOOR_OPEN_BOTTOM) DOOR_OPEN_render(i*32-world->CamX,j*32-world->CamY-32);
		}	
	}
}
void worldSetUp(){
	STONE_setup();
	GRASS_setup();
	DIRT_setup();
	LOG_setup();
	LEAVES_setup();
	PLANKS_setup();
	TORCH_setup();
	COBBLE_setup();
	BEDROCK_setup();
	SAND_setup();
	DOOR_OPEN_setup();
}
