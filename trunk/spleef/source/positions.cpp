#include <nds.h>
#include <dswifi9.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nifi.h"
int bx,by,bfacing,bframe;
unsigned short buffer3[100];
void recievePositionsFromDS(int x,int y,int facing,int frame)
{
	bx=x;
	by=y;
	bfacing=facing;
	bframe=frame;
}
int getbattleX(){ return bx;};
int getbattleY(){ return by;};
int getbattleFacing(){ return bfacing;};
int getbattleFrame(){ return bframe;};
void sendPosition(int x,int y,int facing,int frame)
{
	sprintf((char *)buffer3,"[POS: %d %d %d %d %d", getServerID(),x,y,facing,frame);
	Wifi_RawTxFrame(strlen((char *)buffer3) + 1, 0x0014, buffer3);
}
