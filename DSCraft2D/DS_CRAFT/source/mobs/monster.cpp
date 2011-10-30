#include "monster.h"
#include "../player.h"
#include "../world.h"
#include <stdio.h>
int monsterFrames=0;
void monsterUpdate(monsterActor* monster,worldObject* world,playerActor* player){
	if (monster->monsterPlayer.x < player->x-768 || monster->monsterPlayer.x > player->x+768 ){
		monster->alive=false;
			iprintf("Killed monster at %d,%d\n",monster->monsterPlayer.x,monster->monsterPlayer.y);
	}
	monster->monsterPlayer.person=false;
	updateplayer(&monster->monsterPlayer,world);
	if (monster->monsterPlayer.onblock==true){
		monster->frames_till_jump++;
		if (monster->frames_till_jump%60==0){
			monster->monsterPlayer.vy=-6;
			monster->monsterPlayer.y-=1;
		}
	}
	if (player->x > monster->monsterPlayer.x){
		if (monster->monsterPlayer.onblock==false) monster->monsterPlayer.x+=1;
		monster->monsterPlayer.x+=1;
		monster->monsterPlayer.facing_left=false;
	}
	else if (player->x < monster->monsterPlayer.x){		
		if (monster->monsterPlayer.onblock==false) monster->monsterPlayer.x-=1;
		monster->monsterPlayer.x-=1;
		monster->monsterPlayer.facing_left=true;
	}
	if (monster->monsterPlayer.health<0){
		monster->alive=false;
		iprintf("Killed monster at %d,%d\n",monster->monsterPlayer.x,monster->monsterPlayer.y);
	}
	renderPlayer(&monster->monsterPlayer,world);
	//iprintf("monster pos: %d ,%d\n",monster->monsterPlayer.x,monster->monsterPlayer.y);
}

