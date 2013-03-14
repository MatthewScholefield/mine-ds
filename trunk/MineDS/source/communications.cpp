#include <nds.h>
#include <dswifi9.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nifi.h"
#include "world.h"
#include "mobs/mobHandler.h"
#include "mobs/baseMob.h"
#include "mining.h"
#include "worldRender.h"
int code=0;
int recv_x;
int recv_y;
int framecounter;
int recv_code=2;
//0 = Nothing
//1 = Recieved Handshake, Connected Successfully.
//2 = Revieved Handshake, Game Mismatch. //TODO
//3 = worldTransmit complete!
//4 = Recieved block
int addamount;
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
void recieveWorld(worldObject* world2)
{
	recv_code=0;
	code = 0;
	world = world2;
	unsigned short buffer[100];
	int server_id = getServerID();	
	int client_id = getClientID();
	int i,j;
	for (i=0;i<=16;i++)
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
			j+=addamount;
		}
	}
	recv_x=17;
	recv_y=0;
}
void recieveWorldUpdate()
{

	unsigned short buffer[100];
	int server_id = getServerID();	
	int client_id = getClientID();
	if (recv_code==0)
	{
		framecounter=0;
		code = 0;
		sprintf((char *)buffer,"[BR: %d %d %d %d", server_id, client_id,recv_x,recv_y);
		Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
		recv_code = 1;
	}
	if (recv_code==1)
	{
		if (code == 0)
		{
			framecounter++;
			if (framecounter>30)
			{
				sprintf((char *)buffer,"[BR: %d %d %d %d", server_id, client_id,recv_x,recv_y);
				Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);
				framecounter=0;
			}
		}
		else
		{
			framecounter=0;
			code = 0;
			recv_code = 0;
			recv_y+=addamount+1;
			if (recv_y>WORLD_HEIGHT)
			{
				recv_y=0;
				recv_x++;
			}
			if (recv_x>WORLD_WIDTH)
			{
				recv_code = 2;
			}
		}
	}
}
void communicationInit(worldObject* world2)
{
world=world2;
}
void setBlock(int x,int y,int block,int bgblock,int amount)
{
	int i;
	for (i=y;i<=y+amount;i++)
	{
		world->blocks[x][i]=block;
		world->bgblocks[x][i]=bgblock;
	}
	code = 4;
	addamount=amount;
}
void sendblock(int client_id,int x, int y)
{
	unsigned short buffer[100];
	int server_id = getServerID();	
	int i;
	int a = world->blocks[x][y];
	int b = world->bgblocks[x][y];
	int num=0;
	for (i=y;i<=WORLD_HEIGHT;i++)
	{
		if (world->blocks[x][i]==a && world->bgblocks[x][i]==b)
		{
			 num++;
		} 
		else i = WORLD_HEIGHT+1;
	}
	sprintf((char *)buffer,"[B: %d %d %d %d %d %d %d", server_id, client_id, x, y, world->blocks[x][y],world->bgblocks[x][y],num-1);
	Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
}
void confirmBlock(int client_id,int x,int y)
{
	unsigned short buffer[100];
	int server_id = getServerID();
	sprintf((char *)buffer,"[BLKC: %d %d %d %d %d %d", server_id, client_id, x, y, world->blocks[x][y],world->bgblocks[x][y]);
	Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
}
void clientConfirmBlock(int x,int y)
{
	int client_id = getClientID();
	unsigned short buffer[100];
	int server_id = getServerID();
	sprintf((char *)buffer,"[BLKI: %d %d %d %d %d %d", server_id, client_id, x, y, world->blocks[x][y],world->bgblocks[x][y]);
	Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
}
void matchBlocks(int x,int y,int block,int bgblock)
{
	unsigned short buffer[100];
	int client_id = getClientID();
	int server_id = getServerID();	
	if (world->blocks[x][y]!=block || world->bgblocks[x][y]!=bgblock)
	{
		sprintf((char *)buffer,"[BLKP: %d %d %d %d %d", server_id, x, y, world->blocks[x][y],world->bgblocks[x][y]);
		Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);			
		sprintf((char *)buffer,"[CHKB: %d %d %d %d", server_id, client_id, x, y);
		Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
	}
	else 
	{
		blocksCanPlace();
	}
}
void matchBlocksHost(int client_id,int x,int y,int block,int bgblock)
{
	unsigned short buffer[100];
	int server_id = getServerID();	
	if (world->blocks[x][y]!=block || world->bgblocks[x][y]!=bgblock)
	{
		sprintf((char *)buffer,"[BLKP: %d %d %d %d %d", server_id, x, y, world->blocks[x][y],world->bgblocks[x][y]);
		Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);			
		sprintf((char *)buffer,"[CFMB: %d %d %d %d", server_id, client_id, x, y);
		Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
	}
}
void wifiHurtMob(int mobNum,int amount,int type)
{
	unsigned short buffer[100];
	int server_id = getServerID();
	sprintf((char *)buffer,"[HRT: %d %d %d %d",server_id, mobNum, amount, type);
	Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);		
}
void sendMobUpdater(baseMob* mob,int mobNum)
{
	unsigned short buffer[100];
	int server_id = getServerID();	
	if (mob->alive == true) sprintf((char *)buffer,"[MOB: %d %d %d %d %d %d %d", server_id, mobNum, mob->x, mob->y, mob->animation, mob->mobtype,mob->facing);
	else sprintf((char *)buffer,"[DIE: %d %d", server_id, mobNum);
	Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
}
void recievedMobUpdate(int x,int y,int animation,int mobtype,int mobNum,bool facing)
{
	mobHandlerReadWifiUpdate(x,y,animation,mobtype,mobNum,world,facing);
}
void killMob(int mobNum)
{
	mobHandlerKillMob(mobNum);
}
void recievePlaceBlock(int x,int y,int block,int block2)
{
	world->blocks[x][y]=block;
	world->bgblocks[x][y]=block2;
	updateBrightnessAround(world,x,y);
}
void placeBlock(int x,int y)
{
	if (isHost()==false)
	{
		//Client -> Server Block Placing
		unsigned short buffer[100];
		int server_id = getServerID();	
		int client_id = getClientID();
		sprintf((char *)buffer,"[BLKP: %d %d %d %d %d", server_id, x, y, world->blocks[x][y],world->bgblocks[x][y]);
		Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);			
		sprintf((char *)buffer,"[CHKB: %d %d %d %d", server_id, client_id, x, y);
		Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
	}
	else if (isHost()==true)
	{
		//Client -> Server Block Placing
		unsigned short buffer[100];
		int server_id = getServerID();	
		sprintf((char *)buffer,"[BLKP: %d %d %d %d %d", server_id, x, y, world->blocks[x][y],world->bgblocks[x][y]);
		Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);
		nifiConfirmBlocksAllPlayers(x,y);
		blocksCanPlace();
	}
}
