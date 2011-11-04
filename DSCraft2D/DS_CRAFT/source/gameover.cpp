#include <nds.h>
#include <stdio.h>
#include "ndsvideo.h" // Include the video functions (for setupVideo();)
#include "top-screen.h"
#include "world.h"
#include "player.h"
#include "blockID.h"
#include <stdio.h>

void gameover(worldObject* CurrentWorld, playerActor* MainPlayer){
	int i;
        for (i=0;i<=WORLD_HEIGHT;i++)
                if (CurrentWorld->blocks[WORLD_WIDTH/2][i]!=AIR)
                {
                        MainPlayer->y=i*32-68;
                        MainPlayer->x=WORLD_WIDTHpx/2;
		       	i=WORLD_HEIGHT+1;
			MainPlayer->health=10;
			MainPlayer->person=true;
		}
}

	
