//Circular Dependances RUBBISH (google it(why isn't google a verb))
#ifndef WORLD_H
#define WORLD_WIDTH 31 //32 will be out of bonds  (Also more like chunk width and height)
#define WORLD_HEIGHT 31 //32 wil be out of bounds (trial and error made me realise this (it is pointless))
typedef struct{
	int blocks[WORLD_WIDTH+1][WORLD_HEIGHT+1]; //A world will have 32 by 32 blocks, for now as for testing
	int ChoosedBlock;
	bool DELmode;
	int CamY;
	int CamX;
}worldObject;
void worldUpdate(worldObject* world);
void worldSetUp();
#define WORLD_H
#endif
