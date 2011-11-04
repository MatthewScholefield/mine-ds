#include "mob.h"
#include "player.h"
#include "mobs/monster.h"
#include "world.h"
#include <stdio.h>
int mobs_frame=0;
int mobs_on_screen=0;
mobsStruct Mobs;

bool spawnMonster(){
	int i;
	for (i=0;i<=49;i++){
		if (Mobs.mobs[i].alive==false){
			Mobs.mobs[i].alive=true;
			Mobs.mobs[i].mobPlayer.person=false;
			Mobs.mobs[i].mobPlayer.x=rand() % WORLD_WIDTHpx;
			Mobs.mobs[i].mobPlayer.y=0;
			Mobs.mobs[i].mobPlayer.health=5;
			Mobs.mobs[i].data[0]=1;
			iprintf("Spawned mob at %d,%d\n",Mobs.mobs[i].mobPlayer.x,Mobs.mobs[i].mobPlayer.y);	
			return true;
		}
	}
	iprintf("Failed to Spawn mob\n");
	return false;
}
bool deSpawnMonster(){
	int i;
	for (i=0;i<=49;i++){
		if (Mobs.mobs[i].alive==true && !(Mobs.mobs[i].type==1 && Mobs.mobs[i].data[0]==0)){
			Mobs.mobs[i].alive=false;
			Mobs.mobs[i].mobPlayer.person=false;
			iprintf("DeSpawned mob at %d,%d\n",Mobs.mobs[i].mobPlayer.x,Mobs.mobs[i].mobPlayer.y);	
			return true;
		}
	}
	iprintf("Failed to DeSpawn mob\n");
	return false;
}
void mobUpdate(worldObject* world){
	mobs_frame++;
	if (keysDown() & KEY_SELECT){
		if (!spawnMonster()) iprintf("Failed to spawn a mob\n");
	}if (keysDown() & KEY_B){
		if (!deSpawnMonster()) iprintf("Failed to spawn a mob\n");
	}
	int i;
	for(i=0;i<=49;i++){
		if (Mobs.mobs[i].alive) monsterUpdate(&Mobs.mobs[i],world,&Mobs.mobs[0].mobPlayer);
	}
}
playerActor* mobSetup(){
	int i;
	for(i=0;i<=49;i++) Mobs.mobs[i].alive=false;
	Mobs.mobs[0].alive=true;
	Mobs.mobs[0].type=1;
	Mobs.mobs[0].data[0]=0;
	return &Mobs.mobs[0].mobPlayer;
}
