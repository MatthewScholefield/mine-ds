//Circular Dependances RUBBISH (google it(why isn't google a verb))
#ifndef WORLD_H
#define WORLD_WIDTH 48 //32 will be out of bonds  (Also more like chunk width and height)
#define WORLD_HEIGHT 48 //32 wil be out of bounds (trial and error made me realise this (it is pointless))
#define world_heightpx WORLD_HEIGHT*32
#define world_widthpx WORLD_WIDTH*32
typedef struct{
	int blocks[WORLD_WIDTH+1][WORLD_HEIGHT+1]; //A world will have 32 by 32 blocks, for now as for testing
	int data[WORLD_WIDTH+1][WORLD_HEIGHT+1]; 
	int ChoosedBlock;
	bool DELmode;
	int CamY;
	int CamX;
}worldObject;
void worldUpdate(worldObject* world);
void worldSetUp();
#define WORLD_H
#endif
