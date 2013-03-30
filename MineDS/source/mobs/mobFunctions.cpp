#include <nds.h>
#include <stdio.h>
#include "baseMob.h"
#include "../collision.h"
#include "../blocks.h"
#include "mobcollisions.h"
void calculateMiscData(worldObject* world,baseMob* mob)
{
	if (mob->host)
	{
		if (mob -> collisions[0]==false) mob -> gravityValue--;
		else mob->gravityValue = mob->gravity;
		if (mob -> gravityValue == 0)
		{
			mob -> vy+=1;
			mob -> gravityValue = mob -> gravity;
		}
		int blockx = (mob->x + mob -> sx/2)/16;
		int blocky = (mob->y + 6 + mob -> sy/2)/16;
		bool oldtop= mob -> collisions[0];
		for (int i=0;i<4;i++)
		mob -> collisions[i]=false;
		/*if (spritecol(mob -> x, mob -> y, blockx*16,(blocky+1)*16,mob -> sx, mob ->sy,16,16) && !isBlockWalkThrough(world->blocks[blockx][blocky+1]))
		{
			mob -> collisions[0]=true;
			if (mob -> collisions[0] != oldtop)
			{
				mob -> y = (blocky-1)*16 +1;
			}
		}*/
		int a;

		if (!isBlockWalkThrough(world->blocks[blockx][blocky-1]))
		{
			mob -> collisions[3]=true;
		}
		if (spritecol(mob -> x+2, mob -> y, (blockx+1)*16,(blocky-1)*16,mob -> sx, mob ->sy,16,48) && !(isBlockWalkThrough(world->blocks[blockx+1][blocky]) && isBlockWalkThrough(world->blocks[blockx+1][blocky-1])) )
		{
			mob -> collisions[1]=true;
		}
		if (spritecol(mob -> x-2, mob -> y, (blockx-1)*16,(blocky-1)*16,mob -> sx, mob ->sy,16,48) && !(isBlockWalkThrough(world->blocks[blockx-1][blocky]) && isBlockWalkThrough(world->blocks[blockx-1][blocky-1])) )
		{
			mob -> collisions[2]=true;	
		}
		bool colide=false;
		for (int b=-1;b<=1;b++)
		{
			collision(world,mob,0,blockx,blocky+b,false);
		}
		for (a=0;a<=3;a++)
		{
			if (spritecol(mob -> x+(a-1), mob -> y, (blockx-1+a)*16,(blocky+1)*16,mob -> sx, mob ->sy,16,16) && !isBlockWalkThrough(world->blocks[blockx-1+a][blocky+1]))
			{
				if (a==0 && mob->collisions[2]==false && mob->vy<=0) colide=true;
				else if (a==1) colide=true;
				else if (a==2 && mob->collisions[1]==false && mob->vy<=0) colide=true;
			}
		}
		if (colide)
		{
			mob -> collisions[0]=true;
			mob -> y = (blocky-1)*16 +1;
			mob -> vy = 0;
		}
		if(spritecol(mob -> x,mob -> y, blockx,blocky,mob -> sx,mob -> sy,16,16))
		{
			//Mob is inside the block
			mob -> collisions[4]=true;
			mob -> y = (blocky-1)*16 +1;
			mob -> vy = 0;		
		}
		if (mob->x < 1) mob->x=1;
		if (mob->vy > 6) mob->vy = 6;
	}
}
void calculateMiscDataSmall(worldObject* world,baseMob* mob)
{
	if (mob->host)
	{
		if (mob -> collisions[0]==false) mob -> gravityValue--;
		else mob->gravityValue = mob->gravity;
		if (mob -> gravityValue == 0)
		{
			mob -> vy+=1;
			mob -> gravityValue = mob -> gravity;
		}
		int blockx = (mob->x + mob -> sx/2)/16;
		int blocky = (mob->y + mob -> sy/2)/16;
		bool oldtop= mob -> collisions[0];
		for (int i=0;i<4;i++)
		mob -> collisions[i]=false;
		int a;
		if (!isBlockWalkThrough(world->blocks[blockx][blocky-1]))
		{
			mob -> collisions[3]=true;
		}
		if (spritecol(mob -> x+2, mob -> y, (blockx+1)*16,(blocky-1)*16 + 8,mob -> sx, mob ->sy,16,24) && !(isBlockWalkThrough(world->blocks[blockx+1][blocky])) )
		{
			mob -> collisions[1]=true;
		}
		if (spritecol(mob -> x-2, mob -> y, (blockx-1)*16,(blocky-1)*16 + 8,mob -> sx, mob ->sy,16,24) && !(isBlockWalkThrough(world->blocks[blockx-1][blocky])) )
		{
			mob -> collisions[2]=true;	
		}
		bool colide=false;
		for (int b=-1;b<=1;b++)
		{
			collision(world,mob,0,blockx,blocky+b,false);
		}
		for (a=0;a<=3;a++)
		{
			if (spritecol(mob -> x+(a-1), mob -> y, (blockx-1+a)*16,(blocky+1)*16,mob -> sx, mob ->sy,16,16) && !isBlockWalkThrough(world->blocks[blockx-1+a][blocky+1]))
			{
				if (a==0 && mob->collisions[2]==false && mob->vy<=0) colide=true;
				else if (a==1) colide=true;
				else if (a==2 && mob->collisions[1]==false && mob->vy<=0) colide=true;
			}
		}
		if (colide)
		{
			mob -> collisions[0]=true;
			mob -> y = (blocky)*16 +1;
			mob -> vy = 0;
		}
		if(spritecol(mob -> x,mob -> y, blockx,blocky,mob -> sx,mob -> sy,16,16))
		{
			//Mob is inside the block
			mob -> collisions[4]=true;
			mob -> y = (blocky-1)*16 +1;
			mob -> vy = 0;		
		}
		if (mob->x < 1) mob->x=1;
		if (mob->vy > 6) mob->vy = 6;
	}
}
