#include "world.h"
#include "worldRender.h"
int timeTillChange=0;
int brightness=0;
void timeUpdate(worldObject* world)
{
	timeTillChange++;
	if (timeTillChange>=60)
	{
		world->timeInWorld++;
		timeTillChange=0;
		if (world->timeInWorld>120) world->timeInWorld=0;
		if (world->timeInWorld>=50 && world->timeInWorld<60) brightness++;
		else if (world->timeInWorld>110) 				brightness--;			
		setSun(brightness);	
	}
}

