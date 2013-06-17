#include <nds.h>
#include <stdio.h>
#include "inventoryGraphics.h"
#include "subBgHandler.h"
#include "graphics.h"
#include "../mining.h"
#include "../inventory.h"

Graphic INV[NUM_INV_SPACES];

bool empty = false; //Whether the inventory is empty
int total;
bool loadedInv[NUM_INV_SPACES];
int invID [NUM_INV_SPACES];

void initInvGraphics() //Draws Grey background? Why do that here?, It should be done in a drawing funciton.
{
	/*int i, j;
	for (i=0;i<=24;i++)
		for (j=0;j<=31;j++)
			setSubBgTile(j,i,92);*/
}

void updateInvGraphics() //changes inventory graphics array invenotry slots in case an item is removed or added
{
	int i, j, b = 1;
	total = 0;
	for (i = 1; i <= NUM_INV_SPACES; i++)
	{
		while (checkInventory(b) == 0 && b < NUM_BLOCKS+2)
			b++;
		if (b < NUM_BLOCKS)
			empty = false;
		if (!empty)
		{
			if (loadedInv[i])
				unloadGraphic(&INV[i]);
			loadGraphicSub(&INV[i],2,b);
			//invID[i]=b;            <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< CAUSES INV NOT TO DISPLAY????
			loadedInv[i] = true;
			b++;
			total ++;
		}
	}
}

void drawInv() //Draws the items in the inventory
{
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
		for (a=1;a<=NUM_INV_SPACES;a++)
		{
			//Show sprite image of block
			showGraphic(&INV[a],j,i,false,2);
			//Show tile image of block
			setSubBgTile(j/8,i/8,154);
			setSubBgTile(j/8+1,i/8,154,H_FLIP);
			setSubBgTile(j/8,i/8+1,154,V_FLIP);
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
