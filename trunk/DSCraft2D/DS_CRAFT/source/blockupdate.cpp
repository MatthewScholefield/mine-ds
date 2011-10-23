#include "world.h"
#include "player.h"
#include "allblocks.h"
#include "blockID.h"
void updateBlocks(worldObject* world,playerActor* player){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if (world->blocks[i][j]==TORCH) TORCH_update(i,j,world,player);
			else if (world->blocks[i][j]==GRASS) GRASS_update(i,j,world,player);
			else if (world->blocks[i][j]==DIRT) DIRT_update(i,j,world,player);
		}
}
			
