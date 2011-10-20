#include "world.h"
#include "blockID.h"
#include "allblocks.h"

int i,j;
void worldUpdate(worldObject* world){
	for (i=0;i<=WORLD_WIDTH;i++){//Cycle through the block array
		for (j=0;j<=WORLD_HEIGHT;j++){
			if (world->blocks[i][j]==STONE) STONE_render(i*32-world->CamX,j*32-world->CamY);	
			else if (world->blocks[i][j]==GRASS) GRASS_render(i*32-world->CamX,j*32-world->CamY);	
			else if (world->blocks[i][j]==DIRT) DIRT_render(i*32-world->CamX,j*32-world->CamY);	
			else if (world->blocks[i][j]==LOG) LOG_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==PLACED_LOG) LOG_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==LEAVES) LEAVES_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==PLANKS) PLANKS_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==TORCH) TORCH_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==COBBLE) COBBLE_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==BEDROCK) BEDROCK_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==SAND) SAND_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==DOOR_OPEN_TOP) DOOR_OPEN_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==DOOR_OPEN_BOTTOM) DOOR_OPEN_render(i*32-world->CamX,j*32-world->CamY-32);
			else if (world->blocks[i][j]==DOOR_CLOSED_TOP) DOOR_CLOSED_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==DOOR_CLOSED_BOTTOM) DOOR_CLOSED_render(i*32-world->CamX,j*32-world->CamY-32);
			else if (world->blocks[i][j]==GRAVEL) GRAVEL_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==WHITE_WOOD) WHITE_WOOD_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==DARK_WOOD) DARK_WOOD_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==PLACED_LOG_W) WHITE_WOOD_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==PLACED_LOG_D) DARK_WOOD_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==SANDSTONE) SANDSTONE_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==COAL_ORE) COAL_ORE_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==IRON_ORE) IRON_ORE_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==GOLD_ORE) GOLD_ORE_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==DIAMOND_ORE) DIAMOND_ORE_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==REDSTONE_ORE) REDSTONE_ORE_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==LAPIS_ORE) LAPIS_ORE_render(i*32-world->CamX,j*32-world->CamY);
	        else if (world->blocks[i][j]==GLASS) GLASS_render(i*32-world->CamX,j*32-world->CamY);
			else if (world->blocks[i][j]==PLACED_LEAF) LEAVES_render(i*32-world->CamX,j*32-world->CamY);
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
	DOOR_CLOSED_setup();
	GRAVEL_setup();
	WHITE_WOOD_setup();
	DARK_WOOD_setup();
	SANDSTONE_setup();
	IRON_ORE_setup();
	GOLD_ORE_setup();
	COAL_ORE_setup();
	DIAMOND_ORE_setup();
	REDSTONE_ORE_setup();
	LAPIS_ORE_setup();
	GLASS_setup();
}
