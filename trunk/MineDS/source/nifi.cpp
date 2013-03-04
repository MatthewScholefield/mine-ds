#include <nds.h>
#include <dswifi9.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "graphics/subBgHandler.h"
#include "communications.h"
#include "mining.h"
#include "message.h"
int server_id;
int client_id;
bool host;
bool lookForServers;
bool foundServer;
bool wifiInit=false;
int frame;
bool clientReInit;
int clients[8];
int clientfails[8];
int noOfClients=0;
bool wifiEnabled=false;
unsigned short buffer[100];
int get_int_len (int value){
  int l=1;
  while(value>9){ l++; value/=10; }
  return l;
}
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
			if (lookForServers)
			{
				foundServer=true;
				sscanf(packet,"%*s %d",&server_id); // Server Id contians ARG 1
				lookForServers=false;
			}
		}
	}
	else if (!strcmp("[MSG:",message))
	{
		int test_id;
		sscanf(packet,"%*s %d",&test_id);
		if (test_id == server_id) show_message(&data[39+get_int_len(server_id)]);
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
				if (!strcmp("MineDS",gameName) && noOfClients<8)
				{
					//printf("%d joined the game\n",clients_id);
					//Correct Game, send Accept
					sprintf((char *)buffer,"[ACK: %d %d", server_id, clients_id);
					Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
					noOfClients++;
					int i;
					for (i=0;i<=8;i++)
					{
						if (clients[i]==0)
						{
							clients[i]=clients_id;
							clientfails[i]=0;
							i=10;
						}
					}
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
			sscanf(packet,"%*s %d %d",&test_id, &test2_id);
			if (test_id == server_id && test2_id == client_id) connectSuccess();
			//if (test_id == server_id && test2_id != client_id) printf("%d joined the game\n",test2_id);
		}
	}
	else if (!strcmp("[CHKB:",message))
	{
		if (host == true)
		{
			int test_id;
			int test2_id;
			int x,y;
			sscanf(packet,"%*s %d %d %d %d",&test_id, &test2_id,&x,&y);
			if ( test_id == server_id)
			{
				confirmBlock(test2_id,x,y);
			}
		}
	}
	else if (!strcmp("[CFMB:",message))
	{
		if (host == false)
		{
			int test_id;
			int test2_id;
			int x,y;
			sscanf(packet,"%*s %d %d %d %d",&test_id, &test2_id,&x,&y);
			if ( test_id == server_id && test2_id==client_id)
			{
				clientConfirmBlock(x,y);
			}
		}
	}
	else if (!strcmp("[BLKP:",message))
	{
		int test_id;
		int x,y,block,block2;
		sscanf(packet,"%*s %d %d %d %d %d",&test_id, &x, &y,&block,&block2);
		if ( test_id == server_id)
		{	
			//This Game
			//Set The Block
			recievePlaceBlock(x,y,block,block2);
		}
	}
	else if (!strcmp("[BLKI:",message))
	{
		int test_id,test2_id;
		int x,y,block,block2;
		sscanf(packet,"%*s %d %d %d %d %d %d",&test_id,&test2_id, &x, &y,&block,&block2);
		if ( test_id == server_id)
		{	
			//This Game
			//Set The Block
			matchBlocksHost(test2_id,x,y,block,block2);
		}
	}
	else if (!strcmp("[BLKC:",message))
	{
		if (host == false)
		{
			int test_id,test2_id;
			int x,y,block,block2;
			sscanf(packet,"%*s %d %d %d %d %d %d",&test_id,&test2_id, &x, &y,&block,&block2);
			if ( test_id == server_id && test2_id == client_id)
			{	
				//This Game
				//Set The Block
				matchBlocks(x,y,block,block2);
			}
		}
	}
	else if (!strcmp("[BR:",message))
	{
		if (host == true)
		{
			int test_id;
			int test2_id;
			int x,y;
			client_id=test2_id;
			sscanf(packet,"%*s %d %d %d %d",&test_id, &test2_id, &x, &y);
			if ( test_id == server_id)
			{
				//Respond, we are that server.
				sendblock(test2_id,x, y);
			}
		}
	}
	else if (!strcmp("[B:",message))
	{
		if (host == false)
		{
			int test_id;
			int test2_id;
			int x,y;
			int block_id,bgblock_id;
			int amount;
			sscanf(packet,"%*s %d %d %d %d %d %d %d",&test_id, &test2_id,&x, &y, &block_id, &bgblock_id,&amount);
			if ( test_id == server_id && test2_id == client_id) setBlock(x,y,block_id,bgblock_id,amount);
		}
	}
	else if (!strcmp("[MOB:",message))
	{
		int test_id;
		int a,b,c,d,e,f;
		sscanf(packet,"%*s %d %d %d %d %d %d %d %d",&test_id, &a,&b,&c,&d,&e,&f);
		if ( test_id == server_id) recievedMobUpdate(b,c,d,e,a,f);
	}
	else if (!strcmp("[DIE:",message))
	{
		int test_id;
		int a;
		sscanf(packet,"%*s %d %d",&test_id, &a);
		if ( test_id == server_id) killMob(a);
	}
}
void nifiConfirmBlocksAllPlayers(int x,int y)
{
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
		clientReInit = true;
		int i;
		for (i=0;i<=8;i++)
		{
		 clients[i]=0;
		 clientfails[i]=0;
		}
	}
	else 	Wifi_EnableWifi();
	wifiEnabled=true;
	
	blocksCanPlace();
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
	blocksCanPlace();
}
bool isWifi()
{
	return wifiEnabled;
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
		clientReInit = false;
	}
	if (foundServer)
	{
		iprintf("\x1b[10;0HDo you wish to join server\nID: %d ?\n",server_id);
		drawButton(2,15,5);
		drawButton(15,15,5);
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
				for (i=0;i<=30;i++)
					for(j=14;j<=18;j++)
						setSubBgTile(i,j,0);
				something=false;
				return true;
			}
			else if (keysDown() & KEY_TOUCH)
			{
				iprintf("\x1b[16;4H   ");
				iprintf("\x1b[16;17H  ");
				int i,j;
				iprintf("\x1b[10;0H\x1b[2K\n\x1b[2K"); 
				for (i=0;i<=30;i++)
					for(j=14;j<=18;j++)
						setSubBgTile(i,j,0);
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
		if (frame>=600 + (rand() % 300))
		{
			sprintf((char *)buffer,"[PING: %d", server_id);
			Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);
			frame=0;		
		}
	}
}
