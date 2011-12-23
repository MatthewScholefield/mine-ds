#include "player.h"
#include "world.h"
#include "allblocks.h"
#include "blockID.h"
void updateBlocks(worldObject* world,playerActor* player){
	int i,j;
	for (i=player->blockx-10;i<=player->blockx+10 && i<WORLD_WIDTH;i++)//Cycle through the block array
		for (j=player->blocky-10;j<=player->blocky+10 && j<WORLD_HEIGHT;j++){
			if (i<=-1) i=0;
			if (j<=-1) j=0;
			if (i>WORLD_WIDTH) i=WORLD_WIDTH;
			if (j>WORLD_HEIGHT) j=WORLD_HEIGHT;
			switch(world->blocks[i][j])
			{
				case TORCH: TORCH_update(i,j,world,player); break;
				case FLOWER_RED: FLOWER_RED_update(i,j,world,player); break;
				case FLOWER_YELLOW: FLOWER_YELLOW_update(i,j,world,player); break;
				case GRASS: GRASS_update(i,j,world,player); break;
				case SNOW_GRASS: SNOW_GRASS_update(i,j,world,player); break;
				case DIRT: DIRT_update(i,j,world,player); break;
				case CACTUS: CACTUS_update(i,j,world,player); break;
				case SAND: SAND_update(i,j,world,player); break;
				case GRAVEL: GRAVEL_update(i,j,world,player); break;
			}
		}
}
			
