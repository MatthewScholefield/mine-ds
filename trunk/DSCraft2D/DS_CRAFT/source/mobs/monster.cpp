#include "monster.h"
#include "../player.h"
#include "../world.h"
#include <stdio.h>
int monsterFrames=0;
#include "../colision.h"
#include "monsterGfx.h"
#include "../ndsvideo.h"
u16* monsterGfx[4];
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
		if (player->x > monster->mobPlayer.x && monster->mobPlayer.frame==0){
			if (monster->mobPlayer.onblock==false) monster->mobPlayer.x+=1;
			monster->mobPlayer.x+=1;
			monster->mobPlayer.facing_left=false;
		}
		else if (player->x < monster->mobPlayer.x && monster->mobPlayer.frame==0){		
			if (monster->mobPlayer.onblock==false) monster->mobPlayer.x-=1;
			monster->mobPlayer.x-=1;
			monster->mobPlayer.facing_left=true;
		}
		if (monster->mobPlayer.health<0){
			monster->alive=false;
			iprintf("Killed monster at %d,%d\n",monster->mobPlayer.x,monster->mobPlayer.y);
		}
		if (spritecol(player->x,player->y,monster->mobPlayer.x,monster->mobPlayer.y,32,64,32,64) && monster->mobPlayer.onblock==true){
			playerHurt(player,1,false);
		}	
		if (monster->mobPlayer.frame!=0) monster->mobPlayer.frametime--;
		if (monster->mobPlayer.frametime==0) monster->mobPlayer.frame=0;
		if (!(monster->mobPlayer.frame==2 && monster->mobPlayer.frametime>7)) createsprite32x64(monster->mobPlayer.x-world->CamX,monster->mobPlayer.y-world->CamY,monsterGfx[monster->mobPlayer.frame],monster->mobPlayer.facing_left,4); 
		else createsprite32x64(monster->mobPlayer.x-world->CamX,monster->mobPlayer.y-world->CamY,monsterGfx[0],monster->mobPlayer.facing_left,4); 
	}
	else if (monster->data[0]==0){
		//This is the PlayerMonster not the Mob Monster...
                renderPlayer(player,world);	
	}
}
void monster_setup(){
 char* monstertiles;
 monstertiles=(char*)&monsterGfxTiles;
 monsterGfx[0]=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
 dmaCopy(monstertiles,monsterGfx[0],32*64);
 monstertiles+=32*64;
 monsterGfx[1]=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
 dmaCopy(monstertiles,monsterGfx[1],32*64);
 monstertiles+=32*64;
 monsterGfx[2]=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
 dmaCopy(monstertiles,monsterGfx[2],32*64);
 monstertiles+=32*64;
 monsterGfx[3]=oamAllocateGfx(&oamMain,SpriteSize_32x64, SpriteColorFormat_256Color);
 dmaCopy(monstertiles,monsterGfx[3],32*64);
}

