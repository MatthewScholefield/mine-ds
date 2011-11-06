#include <nds.h>
#include "ndsvideo.h" // Include the video functions (for setupVideo();)
void mainBGUpdate();
void mainBGSetup();
typedef struct time{
	int ticks;
}timeStruct;
timeStruct* timeGet();
void timeSet(timeStruct* time);
