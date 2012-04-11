/* _____________________________________________
| |
| World.h |
| Part of Mine DS , by CoolAs and Ray |
| Thanks to Dirbaio! |
|_____________________________________________|
*/



//Circular Dependances RUBBISH (google it(why isn't google a verb))
#ifndef WORLD_H
#define WORLD_HEIGHT 128
#define world_heightpx WORLD_HEIGHT*16
#define world_widthpx WORLD_WIDTH*16
#define WORLD_WIDTHpx WORLD_WIDTH*16
#define WORLD_WIDTH 1024 //Chuncks are 16x16 (one screen size)
typedef struct{
	int blocks[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int data[WORLD_WIDTH+1][WORLD_HEIGHT+1]; //Plus1??? So If something happens the DS does not read outside of the array
	int brightness[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int lightemit[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int ChoosedBlock;
	bool DELmode;
	int CamY;
	int CamX;
	int version; //To keep track of the world version number
}worldObject;
void worldUpdate(worldObject* world,void* player2);
void worldSetUp();
#define WORLD_H
#endif
