#include "world.h"
#include "worldRender.h"
#include "blockID.h"
#include <nds.h>
#include <stdio.h>
void miningUpdate(worldObject* world,int a,int b,touchPosition touch,int keys)
{
	if (keys & KEY_TOUCH)
	{
		int x = (touch.px+a)/16;
		int y = (touch.py+b)/16;
		//iprintf("Pen touched at: %d,%d\n",x,y);
		iprintf("Block: %d\n",world->blocks[x][y]);
		if (world->blocks[x][y]!=TORCH)
		{
			world->blocks[x][y]=TORCH;
			world->lightemit[x][y]=1;
			brightnessSpread(world,x,y,0);
		}
		else
		{
			world->blocks[x][y]=AIR;
			world->lightemit[x][y]=0;
			updateBrightnessAround(world,x,y);
		}
	}
}
