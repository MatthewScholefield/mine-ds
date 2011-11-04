#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include "player.h"
#include "world.h"
#include "day-night.h"
bool fatinited;
touchPosition touch2;
void saveMenu(worldObject* world){
	lcdSwap();
	worldObject STUF;
	timeStruct STUFF;
	bool fertig=false;
	while(!fertig){
		scanKeys();
		if (keysDown() & KEY_TOUCH){	
			touchRead(&touch2);
			if (touch2.px>=138 && touch2.px<=180 && touch2.py>=27 && touch2.py<=46){
				//Load World1	
				if (fatinited){
					timeStruct* worldTime= timeGet();
					FILE* save_file = fopen("world_1_w.sav", "rb");		
					fread(world, 1, sizeof(STUF), save_file);
					fread(worldTime, 1, sizeof(STUFF), save_file);
					fclose(save_file);
				}		
			}
			else if (touch2.px>=187 && touch2.px<=227 && touch2.py>=27 && touch2.py<=46){
				//Load World1	
				if (fatinited){
					timeStruct* worldTime= timeGet();
					FILE* save_file = fopen("world_1_w.sav", "wb");		
					fwrite(world, 1, sizeof(STUF), save_file);
					fwrite(worldTime, 1, sizeof(STUFF), save_file);
					fclose(save_file);
				}		
			}
			fertig=true;
		}
	}
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
