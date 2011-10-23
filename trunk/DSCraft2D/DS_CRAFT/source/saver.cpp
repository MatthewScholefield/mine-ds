#include <nds.h>
#include <fat.h>
#include "player.h"
#include "world.h"
bool fatinited;
touchPosition touch2;
void saveMenu(){
	lcdSwap();
	bool fertig=false;
	while(!fertig){
		scanKeys();
		if (keysDown() & KEY_TOUCH){	
			touchRead(&touch2);
			if (touch2.px>=138 && touch2.px<=180 && touch2.py>=27 && touch2.py<=46){
				//Load World1			
			}
			fertig=true;
		}
	}
	lcdMainOnBottom();
}
void saveUpdate(worldObject* world,playerActor* player){
	if (keysHeld() & KEY_START){
		saveMenu();
	}
}
void saveInit(){
	fatinited=fatInitDefault();
}
