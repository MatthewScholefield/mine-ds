#pragma once
#include <time.h>

//Do not change WORLD_HEIGHT or WORLD_WIDTH unless you are prepared to update assembly code
//Assembly functions in worldfunctions.s depend on these values being as they are
//Also do not change order of arrays in worldObject struct, just append things to the bottom
#define WORLD_HEIGHT	(128)
#define WORLD_WIDTH	(512)
#define WORLD_HEIGHTPX	(WORLD_HEIGHT * 16)
#define WORLD_WIDTHPX	(WORLD_WIDTH * 16)

enum gamemode_t {
	GAMEMODE_CREATIVE = 1,
	GAMEMODE_SURVIVAL = 2,
	GAMEMODE_PREVIEW = 3
};

typedef struct
{
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
	gamemode_t gamemode;
	int seed; //The random number seed used to generate the world
	double CamCalcX;
	double CamCalcY;
} worldObject;

int findFirstBlock(worldObject* world,int x);
int findFirstBiomeBlock(worldObject* world,int x);
void drawLineDown(worldObject* world,int x, int y);
