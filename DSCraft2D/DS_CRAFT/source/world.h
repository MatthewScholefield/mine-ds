//Circular Dependances RUBBISH (google it(why isn't google a verb))
#ifndef WORLD_H
#define WORLD_HEIGHT 128
#define world_heightpx WORLD_HEIGHT*32
#define world_widthpx WORLD_WIDTH*32
#define WORLD_WIDTH 128
typedef struct{
	int blocks[WORLD_WIDTH+16][WORLD_HEIGHT+16]; //A world will have 32 by 32 blocks, for now as for testing
	int data[WORLD_WIDTH+16][WORLD_HEIGHT+16];
	int ChoosedBlock;
	bool DELmode;
	int CamY;
	int CamX;
}worldObject;

void worldUpdate(worldObject* world,void* player2);
void worldSetUp();
#define WORLD_H
#endif
