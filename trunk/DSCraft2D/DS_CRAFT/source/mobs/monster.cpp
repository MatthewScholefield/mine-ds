#include "monster.h"
#include "../player.h"
#include "../world.h"
#include <stdio.h>
int monsterFrames=0;
#include "../colision.h"
void monsterUpdate(mobActor* monster,worldObject* world,playerActor* player){
	updateplayer(&monster->mobPlayer,world);
	if (!(monster->mobPlayer.x < player->x-320 || monster->mobPlayer.x > player->x+320) && monster->data[0]==1){
		if (monster->mobPlayer.onblock==true){
			monster->data[1]++;
			if (monster->data[1]%60==0){
				monster->mobPlayer.vy=-6;
				monster->mobPlayer.y-=1;
			}
		}
		if (player->x > monster->mobPlayer.x){
			if (monster->mobPlayer.onblock==false) monster->mobPlayer.x+=1;
			monster->mobPlayer.x+=1;
			monster->mobPlayer.facing_left=false;
		}
		else if (player->x < monster->mobPlayer.x){		
			if (monster->mobPlayer.onblock==false) monster->mobPlayer.x-=1;
			monster->mobPlayer.x-=1;
			monster->mobPlayer.facing_left=true;
		}
		if (monster->mobPlayer.health<0){
			monster->alive=false;
			iprintf("Killed monster at %d,%d\n",monster->mobPlayer.x,monster->mobPlayer.y);
		}
		renderPlayer(&monster->mobPlayer,world);
	}
	else if (monster->data[0]==0){
		//This is the PlayerMonster not the Mob Monster...
                renderPlayer(player,world);	
	}
}

