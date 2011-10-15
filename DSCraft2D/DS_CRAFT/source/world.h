//Circular Dependances RUBBISH (google it(why isn't google a verb))
#ifndef WORLD_H
typedef struct{
	int blocks[32][32]; //A world will have 32 by 32 blocks, for now as for testing
	int CamY;
	int CamX;
}worldObject;
void worldUpdate(worldObject* world);
void worldSetUp();
#define WORLD_H
#endif
