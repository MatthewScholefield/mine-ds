#ifndef WORLD_H
#define WORLD_HEIGHT 128
#define world_heightpx WORLD_HEIGHT*16
#define world_widthpx WORLD_WIDTH*16
#define WORLD_WIDTH 512
typedef struct{
	int blocks[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int data[WORLD_WIDTH+1][WORLD_HEIGHT+1]; 
	int brightness[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int lightemit[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int sun[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int bgblocks[WORLD_WIDTH+1][WORLD_HEIGHT+1];
	int CamY;
	int CamX;
	int timeInWorld;
        int worldBrightness;
        bool returnToGame; //Whether world has been generated and the back key will allow going back to it
}worldObject;
int findFirstBlock(worldObject* world,int x);
int findFirstBiomeBlock(worldObject* world,int x);
void drawLineDown(worldObject* world,int x, int y);
#define WORLD_H
#endif
