#include "player.h"
#include "structures.h"
#include "world.h"
#include "allblocks.h"
#include "blockID.h"
void updateBlocks(worldObject* world,playerActor* player){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)//Cycle through the block array
		for (j=0;j<=WORLD_HEIGHT;j++){
			if (i<=-1) i=-1;
			if (j<=-1) j=-1;
			switch(world->blocks[i][j])
			{
				case TORCH: TORCH_update(i,j,world,player); break;
				case GRASS: GRASS_update(i,j,world,player); break;
				case DIRT: DIRT_update(i,j,world,player); break;
			}
		}
}
			
