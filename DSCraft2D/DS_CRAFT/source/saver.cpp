#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include "player.h"
#include "world.h"
#include "day-night.h"
#include "controls.h"
bool fatinited;
touchPosition save_touch;
void saveMenu(worldObject* world){
	lcdSwap();
	bool fertig=false;
	while(!fertig){
		scanKeys();
		if (keysDown() & KEY_TOUCH){	
			touchRead(&save_touch);
			if (save_touch.px>=138 && save_touch.px<=180 && save_touch.py>=27 && save_touch.py<=46){
				//Load World1	
				//if (fatinited){
					iprintf("BLAH\n");
					timeStruct worldTime;
					worldObject world2;
					FILE* save_file = fopen("world_1_w.sav", "rb");		
					fread(world, 1, sizeof(world2), save_file);
					//fread(&worldTime,1,sizeof (timeStruct), save_file);
					timeSet(&worldTime);
					fclose(save_file);
				//}		
			}
			else if (save_touch.px>=187 && save_touch.px<=227 && save_touch.py>=27 && save_touch.py<=46){
				//Load World1	
				//if (fatinited){

					iprintf("BLAH\n");
					timeStruct* worldTime= timeGet();
					FILE* save_file = fopen("world_1_w.sav", "wb");		
					fwrite(world, 1, sizeof(worldObject), save_file);
					//fwrite(worldTime, 1, sizeof(timeStruct), save_file);
					fclose(save_file);
				//}		
			}
			fertig=true;
		}
	}
	timeStruct worldTime;
	worldObject world2;
	FILE* save_file = fopen("world_1_w.sav", "rb");		
	fread(world, 1, sizeof(world2), save_file);
	//fread(&worldTime,1,sizeof (timeStruct), save_file);
	timeSet(&worldTime);
	fclose(save_file);
	int i;
	for (i=0;i<=60;i++) swiWaitForVBlank();
	lcdMainOnBottom();
}
void saveUpdate(worldObject* world,playerActor* player){
	if (keysHeld() & KEY_START){
		saveMenu(world);
	}
}
void saveInit(){
	fatinited=fatInitDefault();
}
