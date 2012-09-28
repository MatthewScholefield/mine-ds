#include <nds.h>
#include <dswifi9.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nifi.h"
int code=0;

//0 = Nothing
//1 = Recieved Handshake, Connected Successfully.
//2 = Revieved Handshake, Game Mismatch. //TODO
//3 = worldTransmit complete!
//4 = Recieved block
int addamount;
int doHandshake()
{
	unsigned short buffer[100];
	int server_id = getServerID();	
	int client_id = getClientID();
	code = 0;
	sprintf((char *)buffer,"[REQ: %d %d Spleef", server_id, client_id);
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
void setBlock(int x,int y)
{
	unsigned short buffer[100];
	sprintf((char *)buffer,"[BRK: %d %d %d", getServerID(), x, y);
	Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
 
}
