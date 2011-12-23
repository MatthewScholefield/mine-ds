#include "world.h"
#include "blockID.h"
#include "allblocks.h"
#include "player.h"
int i,j;
void worldUpdate(worldObject* world,void* player2){
	playerActor* player=(playerActor*)player2;
	for (i=player->blockx-16;i<=player->blockx+16 && i<=WORLD_WIDTH;i++){//Cycle through the block array
		for (j=player->blocky-16;j<=player->blocky+16 && j<=WORLD_HEIGHT;j++){
			if (i<=-1) i=-1;
			if (j<=-1) j=-1;
			if (world->blocks[i][j]!=AIR){
				switch(world->blocks[i][j])
				{
					case STONE: STONE_render(i*32-world->CamX,j*32-world->CamY); break;
					case GRASS: GRASS_render(i*32-world->CamX,j*32-world->CamY); break;	
					case SNOW_GRASS: SNOW_GRASS_render(i*32-world->CamX,j*32-world->CamY); break;	
					case DIRT: DIRT_render(i*32-world->CamX,j*32-world->CamY); break;
					case SAND: SAND_render(i*32-world->CamX,j*32-world->CamY,i,j,world);	 break; //Sand needs extra parameters because it needs to "fall"
					case WHITE_WOOD: WHITE_WOOD_render(i*32-world->CamX,j*32-world->CamY); break;
					case DARK_WOOD: DARK_WOOD_render(i*32-world->CamX,j*32-world->CamY); break;
					case PLACED_LOG_W: WHITE_WOOD_render(i*32-world->CamX,j*32-world->CamY); break;
					case PLACED_LOG_D: DARK_WOOD_render(i*32-world->CamX,j*32-world->CamY); break;
					case LOG: LOG_render(i*32-world->CamX,j*32-world->CamY); break;
					case PLACED_LOG: LOG_render(i*32-world->CamX,j*32-world->CamY); break;
					case LEAVES: LEAVES_render(i*32-world->CamX,j*32-world->CamY); break;
					case PLANKS: PLANKS_render(i*32-world->CamX,j*32-world->CamY); break;
					case TORCH: TORCH_render(i*32-world->CamX,j*32-world->CamY); break;
					case COBBLE: COBBLE_render(i*32-world->CamX,j*32-world->CamY); break;
					case BEDROCK: BEDROCK_render(i*32-world->CamX,j*32-world->CamY); break;
					case DOOR_OPEN_TOP: DOOR_OPEN_render(i*32-world->CamX,j*32-world->CamY); break;
					case DOOR_OPEN_BOTTOM: DOOR_OPEN_render(i*32-world->CamX,j*32-world->CamY-32); break;
					case DOOR_CLOSED_TOP: DOOR_CLOSED_render(i*32-world->CamX,j*32-world->CamY); break;
					case DOOR_CLOSED_BOTTOM: DOOR_CLOSED_render(i*32-world->CamX,j*32-world->CamY-32); break;
					case GRAVEL: GRAVEL_render(i*32-world->CamX,j*32-world->CamY,i,j,world); break;
					case SANDSTONE: SANDSTONE_render(i*32-world->CamX,j*32-world->CamY); break;
					case COAL_ORE: COAL_ORE_render(i*32-world->CamX,j*32-world->CamY); break;
					case IRON_ORE: IRON_ORE_render(i*32-world->CamX,j*32-world->CamY); break;
					case GOLD_ORE: GOLD_ORE_render(i*32-world->CamX,j*32-world->CamY); break;
					case DIAMOND_ORE: DIAMOND_ORE_render(i*32-world->CamX,j*32-world->CamY); break;
					case REDSTONE_ORE: REDSTONE_ORE_render(i*32-world->CamX,j*32-world->CamY); break;
					case LAPIS_ORE: LAPIS_ORE_render(i*32-world->CamX,j*32-world->CamY); break;
					case GLASS: GLASS_render(i*32-world->CamX,j*32-world->CamY); break;
					case CACTUS: CACTUS_render(i*32-world->CamX,j*32-world->CamY); break;
				    //Here Lapis-BLOCK
					case WOOL_WHITE: WOOL_WHITE_render(i*32-world->CamX,j*32-world->CamY); break;
					case PLACED_LEAF: LEAVES_render(i*32-world->CamX,j*32-world->CamY); break;
					case FLOWER_RED: FLOWER_RED_render(i*32-world->CamX,j*32-world->CamY); break;
					case FLOWER_YELLOW: FLOWER_YELLOW_render(i*32-world->CamX,j*32-world->CamY); break;
				}
			}
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
	CACTUS_setup();
	//Here Lapis-BLOCK
	WOOL_WHITE_setup();
	SNOW_GRASS_setup();
	FLOWER_RED_setup();
	FLOWER_YELLOW_setup();
}
