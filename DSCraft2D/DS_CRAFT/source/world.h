//Circular Dependances RUBBISH (google it(why isn't google a verb))
#ifndef WORLD_H
#define WORLD_HEIGHT 128
#define world_heightpx WORLD_HEIGHT*32
#define world_widthpx WORLD_WIDTH*32
#define WORLD_WIDTHpx WORLD_WIDTH*32
#define WORLD_WIDTH 1028
typedef struct{
	int blocks[WORLD_WIDTH+1][WORLD_HEIGHT+1]; //A world will have 32 by 32 blocks, for now as for testing XD???? HOW OUTDATED IS THIS?!?!
	int data[WORLD_WIDTH+1][WORLD_HEIGHT+1]; //Plus16??? So If something happens the DS does not read outside of the array
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
