#include <nds.h>
#include <stdio.h>
#include "inventoryGraphics.h"
#include "subBgHandler.h"
#include "graphics.h"
#include "../mining.h"
#include "../inventory.h"
#include "../message.h"
Graphic INV[NUM_INV_SPACES];

bool empty = false; //Whether the inventory is empty
int total;
bool loadedInv[NUM_INV_SPACES];
int loadedID[NUM_INV_SPACES];
bool enabled = true;
void initInvGraphics()
{
}

void updateInvGraphics()
{

}
void enableInvGraphics()
{
	enabled = true;
}
void disableInvGraphics()
{
	enabled = false;
}
void drawInv() //Draws the items in the inventory (called by the mainPlayer->update if in survival mode)
{
	if (enabled ==false)
		return;
	int i,j;
	for (i=0;i<32;i++)
	{
		for (j=8;j<15;j++)
		{
			setSubBgTile(i,j,28+(i%2));
			if (i==0 && j==8)
				setSubBgTile(i,j,26);
			else if (i==31 && j==8)
				setSubBgTile(i,j,26,H_FLIP);
			else if (i==0 && j==14)
				setSubBgTile(i,j,26,V_FLIP);
			else if (i==31 && j==14)
				setSubBgTile(i,j,26,BOTH_FLIP);
			else if (i==0)
				setSubBgTile(i,j,27);
			else if (i==31)
				setSubBgTile(i,j,27,H_FLIP);
			else if (j==8)
				setSubBgTile(i,j,30);
			else if (j==14)
				setSubBgTile(i,j,30,V_FLIP);
		}
	}
	if (!empty)
	{
		int a;
		j = 8; //start x
		i = 72; //start y
		for (a=0;a<NUM_INV_SPACES;a++)
		{
			if (getBlockID(a) != loadedID[a])
			{
				//Need to reload graphic
				if(loadedInv[a]==true)
					unloadGraphic(&INV[a]);
				else loadedInv[a]=true;
				loadGraphicSub(&INV[a],2,getBlockID(a));
				loadedID[a] = getBlockID(a);
			}
			//Show sprite image of block
			showGraphic(&INV[a],j,i,false,2);
			//Show tile image of block
			setSubBgTile(j/8,i/8,154);
			setSubBgTile(j/8+1,i/8,154,H_FLIP);
			setSubBgTile(j/8,i/8+1,154,V_FLIP);
			if( getBlockAmount(a)!=0)
			{
				if (getBlockAmount(a)>9)
					printf("\x1b[%d;%dH%d",i/8+1,j/8,(int)(getBlockAmount(a)/10));
				else
					printf("\x1b[%d;%dH%c",i/8+1,j/8,' ');
				printf("\x1b[%d;%dH%d",i/8+1,j/8+1,(int)(getBlockAmount(a)%10));
			}	
			else
				printf("\x1b[%d;%dH%c",i/8+1,j/8+1,' ');
			setSubBgTile(j/8+1,i/8+1,154,BOTH_FLIP);
			if (j<232)
				j += 16; //increment x
			else
			{
				j = 8; //Exact same as start x
				i += 24; //increment y
			}
		}
	}
}
