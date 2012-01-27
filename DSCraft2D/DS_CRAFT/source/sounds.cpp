#include <nds.h>
#include <maxmod9.h>    // Maxmod definitions for ARM9
#include "soundbank.h"  // Soundbank definitions
#include "soundbank_bin.h"
#include "sounds.h"
#include <stdio.h>
bool maxModInit=false;
musicStruct music;
void initSounds(){
  	mmInitDefaultMem( (mm_addr)soundbank_bin );
	mmLoadEffect( SFX_GRASS_A );
	mmLoadEffect( SFX_GRASS_B );
	mmLoadEffect( SFX_GRASS_C );
	mmLoadEffect( SFX_GRASS_D );
	mmLoadEffect( SFX_STONE_A );
	mmLoadEffect( SFX_STONE_B );
	mmLoadEffect( SFX_STONE_C );
	mmLoadEffect( SFX_STONE_D );
	mmLoadEffect( SFX_GRAVEL_A );
	mmLoadEffect( SFX_GRAVEL_B );
	mmLoadEffect( SFX_GRAVEL_C );
	mmLoadEffect( SFX_GRAVEL_D );
	mmLoadEffect( SFX_HURT );
	mmLoadEffect( SFX_WOOD_A);
	mmLoadEffect( SFX_WOOD_B);
	mmLoadEffect( SFX_WOOD_C);
	mmLoadEffect( SFX_WOOD_D);
	mmLoadEffect( SFX_SAND_A);
	mmLoadEffect( SFX_SAND_B);
	mmLoadEffect( SFX_SAND_C);
	mmLoadEffect( SFX_SAND_D);
	mmLoadEffect( SFX_PIG_A);
    mmLoad(MOD_CALM);
	mmLoad(MOD_LIVINGMICE);
	mmLoad(MOD_WETHANDS);
	music.musictype=rand()%3;
	music.volume=0;
	music.playing=false;
	music.frames=0;
	music.volumechanging=0; //0 for no changing, 1 for fade in, 2 for fade out...
	maxModInit=true;
}
void playSound(int sound){
	if (maxModInit==false) initSounds();
	switch(sound)
	{
	case:GRASS_A mmEffect( SFX_GRASS_A );break;
	case:GRASS_B mmEffect( SFX_GRASS_B ); break;
	case:GRASS_C mmEffect( SFX_GRASS_C ); break;
	case:GRASS_D mmEffect( SFX_GRASS_D ); break;
	case:STONE_A mmEffect( SFX_STONE_A ); break;
	case:STONE_B mmEffect( SFX_STONE_B ); break;
	case:STONE_C mmEffect( SFX_STONE_C ); break;
	case:STONE_D mmEffect( SFX_STONE_D ); break;
	case:GRAVEL_A mmEffect( SFX_GRAVEL_A ); break;
	case:GRAVEL_B mmEffect( SFX_GRAVEL_B ); break;
	case:GRAVEL_C mmEffect( SFX_GRAVEL_C ); break;
	case:GRAVEL_D mmEffect( SFX_GRAVEL_D ); break;
	case:HURT mmEffect( SFX_HURT ); break;
	case:WOOD_A mmEffect(SFX_WOOD_A); break;
	case:WOOD_B mmEffect (SFX_WOOD_B); break;
	case:WOOD_C mmEffect(SFX_WOOD_C); break;
	case:WOOD_D mmEffect(SFX_WOOD_D); break;
	case:SAND_A mmEffect(SFX_SAND_A); break;
	case:SAND_B mmEffect (SFX_SAND_B); break;
	case:SAND_C mmEffect(SFX_SAND_C); break;
	case:SAND_D mmEffect(SFX_SAND_D); break;
	case:PIG_A mmEffect(SFX_PIG_A); break;
	}
}
void soundUpdate(){
	music.frames++;
	if (music.frames==30){
		if(rand()%256==0 ){
			//iprintf("BLARG");
			if (music.playing==false){
				music.playing=true;
				//iprintf("Start");
				music.volumechanging=1;
				music.musictype=rand() %3;
				music.volume=0;
				if (music.musictype==0) mmStart(MOD_CALM,MM_PLAY_ONCE);
				else if (music.musictype==1) mmStart(MOD_LIVINGMICE,MM_PLAY_ONCE);
				else if (music.musictype==2) mmStart(MOD_WETHANDS,MM_PLAY_ONCE);
				mmSetModuleVolume(music.volume);
			}
			else if (music.playing==true && rand() % 2){
				//iprintf("Stop");
				music.playing=false;
				music.volumechanging=2;
				music.volume=1024;
				mmSetModuleVolume(music.volume);
			}

		}
		music.frames=0;
	}
	if (music.volumechanging==1){
		music.volume++;
		mmSetModuleVolume(music.volume);
		if (music.volume==1024) music.volumechanging=0;
	}
	if (music.volumechanging==2){
		music.volume--;
		mmSetModuleVolume(music.volume);
		if (music.volume==0){
			music.volumechanging=0;
			mmStop();
		}
	}
	if (mmActive()==0) music.playing=false;
}
