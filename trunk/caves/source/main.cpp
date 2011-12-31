#include <nds.h>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include "tiles.h"
#include "person.h"
#include "messages.h"
touchPosition touch;
int part_in_adv;
char missions_message[99];
int missions_data[10];
int framecounting;
int person_x;
int person_y;
int person_x_old;
int person_y_old;
int start_x;
u16* GfxPerson;
int start_y;
char mapTiles[16][12];
void makeMap();
void GenCave();
void clearData(){missions_data[0]++;
	int i;
	for(i=0;i<=10;i++) missions_data[i]=0;
}
int main(){
	setBrightness(1,-15);
	RenderInit();
	lcdMainOnBottom();
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	 GfxPerson=oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	dmaCopy(personTiles,GfxPerson,16*16);
	oamInit(&oamMain, SpriteMapping_1D_128, false);
	dmaCopy(personPal, SPRITE_PALETTE, 512);
	int i;
	int j;
	clearData();
	part_in_adv=1;
	consoleDemoInit();
	person_x=12*16;
	person_y=5*16;
	start_x=person_x;
	start_y=person_y;
	srand(time(NULL));
	sprintf(missions_message,"\n");
	makeMap();
	for (i=0;i<=15;i++)
		for(j=0;j<=11;j++)
			RenderTile(i,j,mapTiles[i][j]);
	swiWaitForVBlank();
	framecounting=0;
		for (i=-16;i<=0;i++){
			swiWaitForVBlank();
			swiWaitForVBlank();
			setBrightness(1,i);
		}
	while(1){
		iprintf("%d.",part_in_adv);
		iprintf(" %s",missions[part_in_adv]);
		iprintf("\n\n\n   %s",missions_message);
		touchRead(&touch);
		if (part_in_adv==1){
			if(touch.px>5*16 && touch.px <6*16){
				if (touch.py>5*16 && touch.py<7*16){	
					if (missions_data[0]<=6 && person_x<128){
						if (keysDown() & KEY_TOUCH) missions_data[0]++;
						sprintf(missions_message,messages[1],6-missions_data[0]);
					}
					else if (person_x>=128) sprintf(missions_message,messages[0]);
					if (missions_data[0]==6){
						part_in_adv++;
						missions_data[0]=0;
						sprintf(missions_message,"\n");
						mapTiles[5][5]=AIR;
						mapTiles[5][6]=GRASS;
						mapTiles[5][4]=GRASS;
						mapTiles[4][4]=GRASS;
						mapTiles[6][4]=GRASS;
						mapTiles[5][3]=GRASS;
					}		
				}		
			}
		}
		else if (person_x>4*16  && person_x<6*16 && person_y>4*16 && person_y<6*16 && part_in_adv==2){
			sprintf(missions_message,"\n");
			part_in_adv++;	
			GenCave();	
		}
		else if (part_in_adv==3 && person_x < 128){
			sprintf(missions_message,"\n");
			part_in_adv++;		
		}
		else if (part_in_adv==4 && touch.px>3*16 && touch.px<4*16 && touch.py>5*16 && touch.py < 7*16){
			mapTiles[9][3]=PLATE;
			part_in_adv++;
		}
		else if (part_in_adv==5 && person_x>9*16 && person_x<10*16 && person_y>3*16 && person_y<4*16){
			mapTiles[3][5]=DARK_STONE;
			mapTiles[3][6]=DARK_STONE;
			part_in_adv++;
		}
		else if (part_in_adv==6 && person_x<0){
			part_in_adv++;		
		}
		if (mapTiles[(person_x+6)/16][(person_y+6)/16]>=LIGHT_WATER && mapTiles[(person_x+6)/16][(person_y+6)/16]<=WATER){
			sprintf(missions_message,messages[2]);
			person_x=start_x;
			person_y=start_y;		
		}
		scanKeys();
		if (keysHeld() & KEY_UP) person_y--;
		else if (keysHeld() & KEY_DOWN) person_y++;
		if (keysHeld() & KEY_LEFT) person_x--;
		else if (keysHeld() & KEY_RIGHT) person_x++;
		if (mapTiles[(person_x+8)/16][(person_y+8)/16]==STONE || mapTiles[(person_x+8)/16][(person_y+8)/16]==DOOR){
			person_x=person_x_old;
			person_y=person_y_old;		
		}
		else{
			person_x_old=person_x;
			person_y_old=person_y;
		}
		framecounting++;
		if (framecounting%20==0){
			for (i=0;i<=15;i++)
				for(j=0;j<=11;j++){
					if(mapTiles[i][j]>=LIGHT_WATER && mapTiles[i][j]<=WATER) mapTiles[i][j]=LIGHT_WATER+(rand()%3);
				}	
		}
		for (i=0;i<=15;i++)
			for(j=0;j<=11;j++){
				RenderTile(i,j,mapTiles[i][j]);
			}
		oamSet(&oamMain, 0, person_x, person_y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			GfxPerson, -1, false, false, false, false, false);
		swiWaitForVBlank();
		consoleClear();
		oamUpdate(&oamMain);
	}
	while(1);
	return 1;
}
void makeMap(){
	int i;
	int j;
	for (i=0;i<=15;i++)
		for (j=0;j<=11;j++)
			mapTiles[i][j]=GRASS;
	for (i=0;i<=15;i++){
			mapTiles[i][0]=LIGHT_WATER+(rand()%3);
			mapTiles[i][11]=LIGHT_WATER+(rand()%3);
	}
	for (i=0;i<=11;i++){
		mapTiles[0][i]=LIGHT_WATER+(rand()%3);
		mapTiles[15][i]=LIGHT_WATER+(rand()%3);	
	}
	mapTiles[1][1]=LIGHT_WATER+(rand()%3);
	mapTiles[2][1]=LIGHT_WATER+(rand()%3);	
	mapTiles[3][1]=WATER;
	mapTiles[14][0]=WATER;
	mapTiles[14][1]=WATER;
	mapTiles[13][2]=WATER;
	mapTiles[14][2]=WATER;
	mapTiles[14][3]=WATER;
	mapTiles[13][1]=WATER;
	mapTiles[14][10]=WATER;
	mapTiles[13][10]=WATER;
	mapTiles[14][9]=WATER;
	mapTiles[12][10]=WATER;
	mapTiles[1][2]=WATER;
	mapTiles[1][3]=WATER;
	mapTiles[1][4]=WATER;
	mapTiles[2][2]=WATER;
	mapTiles[1][10]=WATER;
	mapTiles[2][10]=WATER;
	mapTiles[3][10]=WATER;
	mapTiles[1][9]=WATER;
	mapTiles[2][9]=SAND;
	mapTiles[1][8]=SAND;
	mapTiles[2][8]=SAND;
	mapTiles[3][9]=SAND;
	mapTiles[3][8]=SAND;
	mapTiles[4][9]=SAND;
	mapTiles[4][10]=SAND;
	mapTiles[5][10]=SAND;
	mapTiles[6][10]=SAND;
	mapTiles[5][5]=DIRT;
	mapTiles[5][6]=DIRT;
	mapTiles[5][4]=DARK_GRASS;
	mapTiles[4][4]=DARK_GRASS;
	mapTiles[6][4]=DARK_GRASS;
	mapTiles[5][3]=DARK_GRASS;
	mapTiles[9][6]=WATER;
	mapTiles[10][6]=WATER;
	mapTiles[11][6]=WATER;
	mapTiles[9][5]=WATER;
	mapTiles[10][5]=WATER;
	mapTiles[11][5]=WATER;
}
void GenCave(){

	int i;
	int j;
	for (i=0;i>=-16;i--){
		swiWaitForVBlank();
		swiWaitForVBlank();
		setBrightness(1,i);
	}
	for (i=0;i<=15;i++)
		for (j=0;j<=11;j++)
			mapTiles[i][j]=STONE;
	for (i=0;i<=15;i++){
		mapTiles[i][5]=DARK_STONE;
		mapTiles[i][6]=DARK_STONE;
	}
	mapTiles[8][4]=DARK_STONE;
	mapTiles[7][4]=DARK_STONE;
	mapTiles[6][4]=DARK_STONE;
	mapTiles[9][4]=DARK_STONE;
	mapTiles[8][3]=DARK_STONE;
	mapTiles[7][3]=DARK_STONE;
	mapTiles[6][3]=DARK_STONE;
	mapTiles[9][3]=DARK_STONE;
	mapTiles[3][5]=DOOR;
	mapTiles[3][6]=DOOR;
	for (i=0;i<=15;i++)
		for(j=0;j<=11;j++)
			RenderTile(i,j,mapTiles[i][j]);
	person_x=15*16;
	person_y=5*16+8;
		oamSet(&oamMain, 0, person_x, person_y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			GfxPerson, -1, false, false, false, false, false);
		swiWaitForVBlank();
	oamUpdate(&oamMain);
	for (i=-16;i<=0;i++){
		swiWaitForVBlank();
		swiWaitForVBlank();
		setBrightness(1,i);
	}
	start_x=person_x;
	start_y=person_y;
}
void GenDungeon(){
	
}
