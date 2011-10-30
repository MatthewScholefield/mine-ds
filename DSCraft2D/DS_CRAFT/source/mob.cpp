#include "mob.h"
#include "player.h"
#include "mobs/monster.h"
#include "world.h"
#include <stdio.h>
int mobs_frame=0;
int mobs_on_screen=0;
monsterActor monsters[3];
bool spawnMonster(playerActor* player){
	int i;
	for (i=0;i<=2;i++){
		if (monsters[i].alive==false){
			monsters[i].alive=true;
			monsters[i].monsterPlayer.x=player->x- (rand() % 1024 - 512);
			monsters[i].monsterPlayer.y=0;
			monsters[i].monsterPlayer.health=5;
			iprintf("Spawned mob at %d,%d\n",monsters[i].monsterPlayer.x,monsters[i].monsterPlayer.y);	
			return true;
		}
	}
	iprintf("Failed to Spawn mob\n");
	return false;
}
void mobUpdate(worldObject* world,playerActor* player){
	mobs_frame++;
	if (mobs_frame% 600==0 ) spawnMonster(player);
	if (keysDown() & KEY_SELECT){
		if (!spawnMonster(player)) iprintf("Failed to spawn a mob\n");
	}
	int i;
	for(i=0;i<=3;i++){
		if (monsters[i].alive) monsterUpdate(&monsters[i],world,player);
	}
}
void mobSetup(){
	int i;
	for(i=0;i<=2;i++) monsters[i].alive=false;
}
