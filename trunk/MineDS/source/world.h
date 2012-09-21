/* _____________________________________________
| |
| World.h |
| Part of Mine DS , by CoolAs and Ray |
| Thanks to Dirbaio! |
|_____________________________________________|
STUFFED UP LOGO :P
*/



//Circular Dependances RUBBISH (google it(why isn't google a verb))
#ifndef WORLD_H
#define WORLD_HEIGHT 128
#define world_heightpx WORLD_HEIGHT*16
#define world_widthpx WORLD_WIDTH*16
#define WORLD_WIDTHpx WORLD_WIDTH*16
#define WORLD_WIDTH 1024 
typedef struct{
	int blocks[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int data[WORLD_WIDTH+1][WORLD_HEIGHT+1]; 
	int brightness[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int lightemit[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	bool sun[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int bgblocks[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int ChoosedBlock;
	bool DELmode;
	int CamY;
	int CamX;
	int version; //To keep track of the world version number
}worldObject;
int findFirstBlock(worldObject* world,int x);
int findFirstBiomeBlock(worldObject* world,int x);
#define WORLD_H
#endif
