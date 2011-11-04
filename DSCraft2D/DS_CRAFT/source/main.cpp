//The main.cpp is the simplist file of all, and it should be
//BUT it depends on many other files that are more complex
#include <nds.h>
#include "ndsvideo.h" // Include the video functions (for setupVideo();)
#include "player.h" // The player fuctions
#include "sprcount.h" //The sprite number counter
#include "world.h" //A World!!!!!
#include "worldgen.h"
#include <stdio.h> //For Rand
#include <time.h>
#include "mining.h"
#include "blockID.h" //The Block ID numbers to a word
#include "top-screen.h"
#include "day-night.h"
#include "blockupdate.h"
#include "saver.h"
#include "gameover.h"

#include "mob.h"
int main(){
        int framecounte=0; //framecount
        setupVideo(); 
        lcdMainOnBottom();
        scanKeys();
        bool debug = false;
        if (keysDown() & KEY_START || keysHeld() & KEY_START) debug=true;
        playerActor* MainPlayer;
	MainPlayer = mobSetup();
        worldObject* CurrentWorld = (worldObject *) calloc(1, sizeof(worldObject));
        srand(time(NULL));
        if (debug) consoleDemoInit();
        generateWorld(CurrentWorld);
        saveInit();
        CurrentWorld->ChoosedBlock = 255;
        CurrentWorld->DELmode = false;
        mainBGSetup();
        if (!debug) subBGSetup();
        //Place the player on the first "non grass" block
        int i;
        for (i=0;i<=WORLD_HEIGHT;i++)
                if (CurrentWorld->blocks[WORLD_WIDTH/2][i]!=AIR)
                {
                        MainPlayer->y=i*32-64;
                        MainPlayer->x=WORLD_WIDTHpx/2;
                        i=WORLD_HEIGHT+1;
                }

        MainPlayer->health=10;
	MainPlayer->person=true;
        while(1){
                scanKeys();
                framecounte++;
                mainBGUpdate();
                playerFrame();
                saveUpdate(CurrentWorld,MainPlayer);

                if (!debug) subLifes(MainPlayer->health);
                if (!debug) subShowBlock(CurrentWorld->ChoosedBlock);

                miningUpdate(CurrentWorld,MainPlayer);
                if (MainPlayer->health <= 0) gameover(CurrentWorld,MainPlayer);
                mobUpdate(CurrentWorld); //The Player is updated as a mob.
                updateBlocks(CurrentWorld,MainPlayer);
                worldUpdate(CurrentWorld,(void*)MainPlayer);
                //if (framecounte%240==0) fixgrass(CurrentWorld);
                swiWaitForVBlank(); //Wait for a VBlank
                oamUpdate(&oamMain); //Update the sprites
                oamUpdate(&oamSub);
                //iprintf("Camera Position:%d,%d\n",CurrentWorld->CamX,CurrentWorld->CamY);
                //iprintf("Player Position:%d,%d\n",MainPlayer->x,MainPlayer->y);
                //iprintf("Player BlockPos:%d,%d\n",MainPlayer->blockx,MainPlayer->blocky);
                //printf("Sprites on Screen: %d\n",nextSprite());
                //iprintf("Choosen Block: %d\n",CurrentWorld->ChoosedBlock);
                resetSpriteCount(); //And set the sprite number counter to 0
                if (framecounte>240) framecounte=1;

        }

}

