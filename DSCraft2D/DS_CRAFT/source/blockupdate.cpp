#include "player.h"
#include "structures.h"
#include "world.h"
#include "allblocks.h"
#include "blockID.h"
void updateBlocks(worldObject* world,playerActor* player){
	int i,j;
	for (i=player->blockx-16;i<=player->blockx+16;i++)//Cycle through the block array
		for (j=player->blocky-16;j<=player->blocky+16;j++){
			if (world->blocks[i][j]==TORCH) TORCH_update(i,j,world,player);
			else if (world->blocks[i][j]==GRASS) GRASS_update(i,j,world,player);
			else if (world->blocks[i][j]==DIRT) DIRT_update(i,j,world,player);
		}
}
			
