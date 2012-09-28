#include <nds.h>
#include <dswifi9.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "communications.h"
#include "positions.h"
#include "mainGame.h"
int server_id;
int client_id;
bool host;
bool lookForServers;
bool foundServer;
bool wifiInit=false;
int frame;
bool clientReInit=true;
int clients[8];
int clientfails[8];
int noOfClients=0;
bool wifiEnabled=false;
bool sendPing=true;
unsigned short buffer[100];
void Handler(int packetID, int readlength)
{
	static char data[4096];
	char message[10];
	Wifi_RxRawReadPacket(packetID, readlength, (unsigned short *)data);
	//data now has the transmitted packet
	char* packet = &data[32]; //Start of real data
	sscanf(packet,"%s",message);
	//Message now contains the first string (the command)
	if (!strcmp("[PING:",message))
	{
		//We Recieved a ping message!
		if (host==false)
		{
			iprintf("A\n");
			if (lookForServers==true)
			{
				iprintf("B\n");
				foundServer=true;
				sscanf(packet,"%*s %d",&server_id); // Server Id contians ARG 1
				lookForServers=false;
			}
		}
	}
	else if (!strcmp("[REQ:",message))
	{
		//We Recieved a ping message!
		if (host==true)
		{

			char gameName[20];
			int test_id;
			int clients_id;
			sscanf(packet,"%*s %d %d %s",&test_id, &clients_id, gameName);
			if (test_id == server_id)
			{
				//Respond, They are talking to us!
				if (!strcmp("Spleef",gameName) )
				{
					//Correct Game, send Accept
					int seed = rand();
					sprintf((char *)buffer,"[ACK: %d %d %d", server_id, clients_id,seed);
					Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
					sendPing=false;
					srand(seed);	
				}
				else 
				{
					//Wrong Game/No Room, send No!!!
					sprintf((char *)buffer,"[NOPE: %d %d", server_id, clients_id);
					Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
					
				}
			}
		}
	}
	else if (!strcmp("[ACK:",message))
	{
		if (host == false)
		{
			int test_id;
			int test2_id;
			int seed;
			sscanf(packet,"%*s %d %d %d",&test_id, &test2_id,&seed);
			if ( test_id == server_id && test2_id == client_id)
			{
			 connectSuccess();
			srand(seed);
			}
		}
	}
	else if (!strcmp("[POS:",message))
	{
		int test_id;
		int x,y,facing,frame;
		sscanf(packet,"%*s %d %d %d %d %d",&test_id, &x,&y,&facing,&frame);
		if ( test_id == server_id)
		{
		 recievePositionsFromDS(x,y,facing,frame);
		 recievedMessage();
		}
	}
	else if (!strcmp("[BRK:",message))
	{
		int test_id;
		int x,y;
		sscanf(packet,"%*s %d %d %d",&test_id, &x,&y);
		if ( test_id == server_id)
		{
		 breakBlock(x,y);
		 recievedMessage();
		}
	}

}

void nifiConfirmBlocksAllPlayers(int x,int y)
{
//LOL
	int i;
	for (i=0;i<=8;i++)
	{
		if (clients[i]!=0)
		{
			//Send confirm to every client
			sprintf((char *)buffer,"[CFMB: %d %d %d %d", server_id, clients[i], x, y);
			Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);
		}

	}
}
int getServerID()
{
	return server_id;
}
int getClientID()
{
	return client_id;
}
void nifiInit()
{

}
void nifiEnable()
{
	if (!wifiInit)
	{	
		Wifi_InitDefault(false);
		Wifi_EnableWifi();
		Wifi_SetPromiscuousMode(1);
		Wifi_RawSetPacketHandler(Handler);
		Wifi_SetChannel(10);
		wifiInit=true;
		int i;
		for (i=0;i<=8;i++)
		{
		 clients[i]=0;
		 clientfails[i]=0;
		}
	}
	else 	Wifi_EnableWifi();
	clientReInit = true;
	wifiEnabled=true;
	sendPing=true;
}
void nifiDisable()
{
	Wifi_DisableWifi();
	for (int i=0;i<=8;i++)
	{
	 clients[i]=0;
	 clientfails[i]=0;
	}	
	clientReInit = true;
	wifiEnabled=false;
	sendPing=true;
}
bool isWifi()
{
	return wifiEnabled;
}
bool startGame()
{
	if (sendPing) return false;
	else return true;
}
bool hostNifiInit()
{
	host = true;
	server_id = rand() % 1024;
	iprintf("Server ID: %d\n",server_id);
	return true;
}
bool clientNifiInit()
{
	host = false;
	if (clientReInit==true)
	{
		client_id = rand() % 1024;
		iprintf("Client ID: %d\n",client_id);
		lookForServers = true;
		foundServer = false;
		clientReInit = false;
	}
	if (foundServer==true)
	{
		iprintf("\x1b[10;0HDo you wish to join server\nID: %d ?\n",server_id);
		iprintf("\x1b[16;4HYes");
		iprintf("\x1b[16;17HNo");
		bool something=true;
		touchPosition touch;
		while (something)
		{
			swiWaitForVBlank();
			scanKeys();
			touchRead(&touch);
			if (keysDown() & KEY_TOUCH && touch.px < 100)
			{
				iprintf("\x1b[16;4H   ");
				iprintf("\x1b[16;17H  ");
				int i,j;
				iprintf("\x1b[10;0H\x1b[2K\n\x1b[2K"); 
				something=false;
				return true;
			}
			else if (keysDown() & KEY_TOUCH)
			{
				iprintf("\x1b[16;4H   ");
				iprintf("\x1b[16;17H  ");
				int i,j;
				iprintf("\x1b[10;0H\x1b[2K\n\x1b[2K"); 
				something=false;
				foundServer=false;
				lookForServers = true;
				return false;			
			}
		}
		
	}
	return false;
}
bool isHost()
{
	return host;
}
void nifiUpdate()
{
	frame++;
	if (host)
	{
		if (frame>=600 && sendPing==true)
		{
			sprintf((char *)buffer,"[PING: %d", server_id);
			Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);
			frame=0;		
		}
	}
}
