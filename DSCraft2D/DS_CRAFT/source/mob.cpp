#include "mob.h"
#include "player.h"
#include "mobs/monster.h"
#include "world.h"
#include "colision.h" 
#include "controls.h"
#include "day-night.h"
#include "mobs/pigMob.h"
#include <fat.h>
#include <stdio.h>
#include <nds.h>
int mobs_frame=0;
int mobs_on_screen=0;
mobsStruct Mobs;
timeStruct* mob_time;
touchPosition touch3;

bool spawnMonster(){
	int i;
	for (i=0;i<=49;i++){
		if (Mobs.mobs[i].alive==false){
			Mobs.mobs[i].mobPlayer.x=Mobs.mobs[0].mobPlayer.x-(rand() % 1536)+768;
			if (rand()%2){
				Mobs.mobs[i].mobPlayer.x=Mobs.mobs[0].mobPlayer.x-rand()%768-96;
			}
			else{
				Mobs.mobs[i].mobPlayer.x=Mobs.mobs[0].mobPlayer.x+rand()%768+96;
			}
			Mobs.mobs[i].alive=true;
			Mobs.mobs[i].mobPlayer.person=false;
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
void mobsSave(FILE* save_file){
	fwrite(&Mobs, 1, sizeof(Mobs), save_file);
}
void mobsLoad(FILE* save_file){
	fread(&Mobs, 1, sizeof(Mobs), save_file);
}
void mobUpdate(worldObject* world){
	mobs_frame++;
	mob_time=timeGet();
        if (mob_time->ticks>100 && mob_time->ticks<250){
		//It is night time
		if (mobs_frame%750==0) spawnMonster();
	}
	//if (keysHeld() & KEY_B){
		//PIG_update(&Mobs.mobs[1],world,&Mobs.mobs[0].mobPlayer);
	//}
	if (keysDown() & KEY_B){
	//	if (!deSpawnMonster()) iprintf("Failed to spawn a mob\n");
	}
	
	int i;
	for(i=0;i<=49;i++){
		if (Mobs.mobs[i].alive){
			monsterUpdate(&Mobs.mobs[i],world,&Mobs.mobs[0].mobPlayer);

		}
	}
 	touch3=controlsTouch();
	int x=touch3.px;
	int y=touch3.py;
	x+=world->CamX;
	y+=world->CamY;
	for (i=1;i<=49;i++){
		if (spritecol(x,y,Mobs.mobs[i].mobPlayer.x,Mobs.mobs[i].mobPlayer.y,2,2,32,64) && x>Mobs.mobs[0].mobPlayer.x-96 && x<Mobs.mobs[0].mobPlayer.x+96 && y>Mobs.mobs[0].mobPlayer.y-96 && y<Mobs.mobs[0].mobPlayer.y+96 ){
			playerHurt(&Mobs.mobs[i].mobPlayer,1,false);				
		}
	}
}
playerActor* mobSetup(){
	int i;
	for(i=0;i<=49;i++) Mobs.mobs[i].alive=false;
	Mobs.mobs[0].alive=true;
	Mobs.mobs[0].type=1;
	Mobs.mobs[0].data[0]=0;
	Mobs.mobs[0].mobPlayer.height=2;
	PIG_setup();
	monster_setup();
	return &Mobs.mobs[0].mobPlayer;
}
