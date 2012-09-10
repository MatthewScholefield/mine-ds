#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include <nds.h>
#include <stdio.h>
#include "graphics/graphics.h"
#define sizeOfArray(x) (sizeof(x)/4)
int blockswithlight[]={TORCH,PUMPKIN_LIGHT};
int selectedblock=0;
Graphic topBlock;
void calculateTopBlock()
{
}
void miningUpdate(worldObject* world,int a,int b,touchPosition touch,int keys)
{
	if (keys & KEY_TOUCH)
	{
		int x = (touch.px+a)/16;
		int y = (touch.py+b)/16;
		int a;
		bool notalightblock=true;
		for (a=0;a<=sizeOfArray(blockswithlight);a++)
		{
			if (selectedblock==blockswithlight[a])
			{
				world->blocks[x][y]=selectedblock;
				if (world->lightemit[x][y]!=1)
				{
					world->lightemit[x][y]=1;
					brightnessSpread(world,x,y,0);
				}
				a = sizeOfArray(blockswithlight);
				notalightblock = false;
			}
		}
		if (notalightblock)
		{
			world->blocks[x][y]=selectedblock;
			if (world->lightemit[x][y]!=0)
			{
				world->lightemit[x][y]=0;
			}
			updateBrightnessAround(world,x,y);
		}
	}
	if (keys & KEY_L)
	{
		selectedblock--;
	}
	else if (keys & KEY_R)
	{
		selectedblock++;
	}
	if (selectedblock<=0) selectedblock=0;
}
