#include <nds.h>
#include <stdio.h>
#include "ndsvideo.h" // Include the video functions (for setupVideo();)
#include "top-screen.h"
#include "world.h"
#include "player.h"
#include "blockID.h"
#include <stdio.h>

void gameover(worldObject* world, playerActor* player){
	int i;
	for (i=0;i<=WORLD_HEIGHT;i++)
		if (world->blocks[0][i]!=AIR)
		{
			player->y=i*32-64;
			player->x=0;
			i=WORLD_HEIGHT+1;
		}
}

	
