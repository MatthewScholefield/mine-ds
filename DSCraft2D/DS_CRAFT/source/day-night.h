#include <nds.h>
#include <stdio.h>
#include "ndsvideo.h" // Include the video functions (for setupVideo();)
void mainBGUpdate();
void mainBGSetup();
typedef struct time{
	int ticks;
	int r;
	int g;
	int b;
}timeStruct;
timeStruct* timeGet();
void timeSet(int time);
void timeSave(FILE* save_file);
void timeLoad(FILE* save_file);
