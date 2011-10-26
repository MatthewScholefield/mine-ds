#include <nds.h>
#include <maxmod9.h>    // Maxmod definitions for ARM9
#include "soundbank.h"  // Soundbank definitions
#include "soundbank_bin.h"
#include "sounds.h"
bool maxModInit=false;
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
	maxModInit=true;
}
void playSound(int sound){
	if (maxModInit==false) initSounds();
	if (sound==GRASS_A) mmEffect( SFX_GRASS_A );
	else if (sound==GRASS_B) mmEffect( SFX_GRASS_B );
	else if (sound==GRASS_C) mmEffect( SFX_GRASS_C );
	else if (sound==GRASS_D) mmEffect( SFX_GRASS_D );
	else if (sound==STONE_A) mmEffect( SFX_STONE_A );
	else if (sound==STONE_B) mmEffect( SFX_STONE_B );
	else if (sound==STONE_C) mmEffect( SFX_STONE_C );
	else if (sound==STONE_D) mmEffect( SFX_STONE_D );
	else if (sound==GRAVEL_A) mmEffect( SFX_GRAVEL_A );
	else if (sound==GRAVEL_B) mmEffect( SFX_GRAVEL_B );
	else if (sound==GRAVEL_C) mmEffect( SFX_GRAVEL_C );
	else if (sound==GRAVEL_D) mmEffect( SFX_GRAVEL_D );
}
