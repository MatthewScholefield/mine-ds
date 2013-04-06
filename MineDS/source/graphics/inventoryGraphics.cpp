#include <nds.h>
#include <stdio.h>
#include "inventoryGraphics.h"
#include "subBgHandler.h"
#include "graphics.h"
#include "../mining.h"
#include "../inventory.h"

Graphic INV[20];

bool empty = false; //Whether the inventory is empty
int total;
bool loadedInv[20];

void initInvGraphics() //Draws Grey background
{
	int i, j;
	for (i=0;i<=24;i++)
		for (j=0;j<=31;j++)
			setSubBgTile(j,i,92);
}

void updateInvGraphics() //changes inventory graphics array invenotry slots in case an item is removed or added
{
	int i, j, b = 1;
	total = 0;
	for (i = 1; i <= 20; i++)
	{
		while (checkInventory(b) == 0 && b < NUM_BLOCKS)
			b++;
		if (b == NUM_BLOCKS)
			empty = true;
		else if (b < NUM_BLOCKS)
			empty = false;
		if (!empty)
		{
			if (loadedInv[i])
				unloadGraphic(&INV[i]);
			loadGraphicSub(&INV[i],2,b);
			loadedInv[i] = true;
			b++;
			total ++;
		}
	}
}

void drawInv() //Draws the items in the inventory
{
	if (!empty)
	{
		int i,j,a;
		j = 60;
		i = 60;
		for (a=1;a<=total;a++)
		{
			showGraphic(&INV[a],j,i);
			if (j<200)
				j += 60;
			else
			{
				j = 70;
				i += 60;
			}
		}
	}
}