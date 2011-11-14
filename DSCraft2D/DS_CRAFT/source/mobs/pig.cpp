#include <nds.h>
#include "../ndsvideo.h"
#include "../mob.h"
#include "../world.h"
#include "../player.h"
#include "pig.h" //Pig images

u16* pigGfx[4]!;

void PIG_update(mobActor* monster,worldObject* world,playerActor* player){
  createsprite64x32(70,70,pigGfx,false,3);
}

void PIG_spawn(){
}

void PIG_setup(){
 char* pigtiles;
 pigTiles=(char*)&pigTiles;
 pigGfx[0]=oamAllocateGfx(&oamMain,SpriteSize_64x32, SpriteColorFormat_256Color);
 dmaCopy(pigtiles,pigGfx[0],64*32);
 pigtiles+=64*32;
 pigGfx[1]=oamAllocateGfx(&oamMain,SpriteSize_64x32, SpriteColorFormat_256Color);
 dmaCopy(pigtiles,pigGfx[1],64*32);
 pigtiles+=64*32;
 pigGfx[2]=oamAllocateGfx(&oamMain,SpriteSize_64x32, SpriteColorFormat_256Color);
 dmaCopy(pigtiles,pigGfx[2],64*32);
 pigtiles+=64*32;
 pigGfx[3]=oamAllocateGfx(&oamMain,SpriteSize_64x32, SpriteColorFormat_256Color);
 dmaCopy(pigtiles,pigGfx[3],64*32);
}