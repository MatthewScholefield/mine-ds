#include <nds.h>
#include "player.h"
#include "world.h"
#include "blockID.h"
#include "gameshelper.h"
touchPosition touch;
int spritecol2(int fx,int fy,int sx,int sy,int fSizex,int fSizey,int sSizex,int sSizey){
	if ((fx + fSizex > sx )&& (fx < sx+sSizex) && (sy + sSizey > fy) && (sy < fy+fSizey)) 
		return 1;
	else 
		return 0;
	return 0;
}
void miningUpdate(worldObject* CurrentWorld,playerActor* MainPlayer){
		scanKeys();
		if (keysDown() & KEY_L && CurrentWorld->DELmode == false){ //Switchting between blocks
			//CurrentWorld->ChoosedBlock-=1; //One block down
		}
		else if (keysDown() & KEY_R && CurrentWorld->DELmode == false){
			//CurrentWorld->ChoosedBlock+=1; //One block up
		}
		if (keysDown() & KEY_SELECT){
		  	if (CurrentWorld->DELmode == false){
		        	CurrentWorld->DELmode = true;
		       		CurrentWorld->ChoosedBlock = 255; //AIR
			}
			else if (CurrentWorld->DELmode == true){
			      CurrentWorld->DELmode = false;
				  CurrentWorld->ChoosedBlock = 1;
			}
		}
		if (keysHeld() & KEY_TOUCH){
			touchRead(&touch);
			int x=touch.px;
			int y=touch.py;
			x+= CurrentWorld->CamX;
			y+=CurrentWorld->CamY;
			int i,j;
			int lax,lay;
			for(i=0;i<=WORLD_WIDTH;i++)
				for(j=0;j<=WORLD_HEIGHT;j++)
					if (spritecol2(x,y,i*32,j*32,1,1,32,32)){
						lax=i;
						lay=j;
						j=WORLD_HEIGHT+2;
						i=WORLD_WIDTH+2;
					}
        		if (CurrentWorld->blocks[lax][lay]!=BEDROCK && (lax!=MainPlayer->blockx || (lay!=MainPlayer->blocky && lay!=MainPlayer->blocky-1))) CurrentWorld->blocks[lax][lay]=CurrentWorld->ChoosedBlock; 
			//the lax!=MainPlayer->**** stuff makes your your not placing a block inside the player
		}
}
