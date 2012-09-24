#include <nds.h>
#include <dswifi9.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nifi.h"
#include "world.h"
int code=0;

//0 = Nothing
//1 = Recieved Handshake, Connected Successfully.
//2 = Revieved Handshake, Game Mismatch. //TODO
//3 = worldTransmit complete!
//4 = Recieved block
worldObject* world;
int doHandshake()
{
	unsigned short buffer[100];
	int server_id = getServerID();	
	int client_id = getClientID();
	code = 0;
	sprintf((char *)buffer,"[REQ: %d %d MineDS", server_id, client_id);
	Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
	int timer = 300;
	//Wait for 5 seconds for a ACK message!
	while (timer > 0 && code == 0)
	{
		timer--;
		swiWaitForVBlank();
	}	
	if (code == 1)
	{
		iprintf("Joined Successfully!\n");
		return 1;
	}
	return 0;
}
void connectSuccess()
{
	code = 1;
}
void connectCode(int code2)
{
	code = code2;
}
int getFirstAirBlock(int x)
{
	
}
void recieveWorld(worldObject* world2)
{
	code = 0;
	world = world2;
	unsigned short buffer[100];
	int server_id = getServerID();	
	int client_id = getClientID();
	int i,j;
	int framecounter;
	for (i=0;i<=WORLD_WIDTH;i++)
	{
		iprintf(".");
		for(j=0;j<=WORLD_HEIGHT;j++)
		{
			framecounter=0;
			code = 0;
			sprintf((char *)buffer,"[BR: %d %d %d %d", server_id, client_id,i,j);
			Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
			while (code == 0)
			{
				framecounter++;
				if (framecounter>30)
				{
					Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);
					framecounter=0;
				}
				swiWaitForVBlank();
			}
		}
	}
}
void communicationInit(worldObject* world2)
{
	world = world2;
}
void setBlock(int x,int y,int block,int bgblock)
{
	world->blocks[x][y]=block;
	world->bgblocks[x][y]=bgblock;
	code = 4;
}
void sendblock(int client_id,int x, int y)
{
	unsigned short buffer[100];
	int server_id = getServerID();	
	sprintf((char *)buffer,"[B: %d %d %d %d %d %d", server_id, client_id, x, y, world->blocks[x][y],world->bgblocks[x][y]);
	Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
}
