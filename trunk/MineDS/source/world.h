#ifndef _WORLD_H_
#define _WORLD_H_

#define WORLD_HEIGHT	(128)
#define WORLD_WIDTH	(512)
#define WORLD_HEIGHTPX	(WORLD_HEIGHT * 16)
#define WORLD_WIDTHPX	(WORLD_WIDTH * 16)

enum gamemode_t {
	GAMEMODE_CREATIVE,
	GAMEMODE_SURVIVAL
};

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
	gamemode_t gamemode;
}worldObject;

int findFirstBlock(worldObject* world,int x);
int findFirstBiomeBlock(worldObject* world,int x);
void drawLineDown(worldObject* world,int x, int y);

#endif /* !_WORLD_H_ */
