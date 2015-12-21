#include <nds.h>
#include <dswifi9.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "nifi.h"
#include "mobs/mobHandler.h"
#include "graphics/graphics.h"
#include "worldRender.h"
#include "graphics/subBgHandler.h"
#include "communications.h"
#include "mining.h"
#include "general.h"
#include "sounds.h"
int server_id;
int client_id;
bool host;
bool lookForServers;
bool foundServer;
int frame;
bool clientReInit;
int clients[MAX_CLIENTS];
int clientfails[MAX_CLIENTS];
int noOfClients;
bool wifiEnabled = false;
unsigned short buffer[100];

int get_int_len(int value)
{
	int l = 1;
	while (value > 9)
	{
		++l;
		value /= 10;
	}
	return l;
}

void nifiClearClients()
{
	int i;
	for (i = 0; i < MAX_CLIENTS; ++i)
	{
		clients[i] = 0;
		clientfails[i] = 0;
	}
	noOfClients = 0;
	clientReInit = true;
}

void Handler(int packetID, int readlength)
{
	bool printmessage = false; //Set to true to see every message (execpt for mob Updates) on the top screen.
	char data[4096];
	char *msgtype, *message;
	Wifi_RxRawReadPacket(packetID, readlength, (unsigned short *) data);
	msgtype = &data[32]; // Start of real data
	message = strchr(msgtype, ' ');
	if (message == nullptr) return; // Invalid message (no space)
	*message++ = '\0';
	// msgtype contains everything before the first space
	// and message contains everything after it
	if (!strcmp("[PING:", msgtype))
	{
		//We Recieved a ping message!
		if (host == false)
		{
			if (lookForServers)
			{
				foundServer = true;
				sscanf(message, "%d", &server_id); // Server Id contians ARG 1
				lookForServers = false;
			}
		}
	}
	else if (!strcmp("[DNC:", msgtype))
	{
		int test_id;
		int sunbrightness;
		int r, g, b;
		sscanf(message, "%d %d %d %d %d", &test_id, &sunbrightness, &r, &g, &b);
		if (test_id == server_id)
		{
			//worldptr->worldBrightness = sunbrightness;
			//setSkyColor(r, g, b);
		}
	}
	else if (!strcmp("[MSG:", msgtype))
	{
		int test_id;
		sscanf(message, "%d", &test_id);
		if (test_id == server_id) printLocalMessage(&data[39 + get_int_len(server_id)]);
	}
	else if (!strcmp("[HRT:", msgtype))
	{
		int test_id;
		int mobNum, amount, type;
		sscanf(message, "%d %d %d %d", &test_id, &mobNum, &amount, &type);
	}
	else if (!strcmp("[SND:", msgtype))
	{
		int test_id;
		int sound;
		sscanf(message, "%d %d", &test_id, &sound);
		if (test_id == server_id) playSoundNifi((Sound) sound);
	}
	else if (!strcmp("[REQ:", msgtype))
	{
		//We Recieved a ping message!
		if (host == true)
		{
			char gameName[20];
			int test_id;
			int clients_id;
			sscanf(message, "%d %d %s", &test_id, &clients_id, gameName);
			if (test_id == server_id)
			{
				//Respond, They are talking to us!
				if (!strcmp("MineDS", gameName) && noOfClients < MAX_CLIENTS)
				{
					//printf("%d joined the game\n",clients_id);
					//Correct Game, send Accept
					sprintf((char *) buffer, "[ACK: %d %d", server_id, clients_id);
					Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
					++noOfClients;
					int i;
					for (i = 0; i < MAX_CLIENTS; ++i)
					{
						if (clients[i] == 0)
						{
							clients[i] = clients_id;
							clientfails[i] = 0;
							break;
						}
					}
				}
				else
				{
					//Wrong Game/No Room, send No!!!
					sprintf((char *) buffer, "[NOPE: %d %d", server_id, clients_id);
					Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
				}
			}
		}
	}
	else if (!strcmp("[ACK:", msgtype))
	{
		if (host == false)
		{
			int test_id;
			int test2_id;
			sscanf(message, "%d %d", &test_id, &test2_id);
			if (test_id == server_id && test2_id == client_id) connectSuccess();
			//if (test_id == server_id && test2_id != client_id) printf("%d joined the game\n",test2_id);
		}
	}
	else if (!strcmp("[CHKB:", msgtype))
	{
		if (host == true)
		{
			int test_id;
			int test2_id;
			int x, y;
			sscanf(message, "%d %d %d %d", &test_id, &test2_id, &x, &y);
			if (test_id == server_id)
				confirmBlock(test2_id, x, y);
		}
	}
	else if (!strcmp("[CFMB:", msgtype))
	{
		if (host == false)
		{
			int test_id;
			int test2_id;
			int x, y;
			sscanf(message, "%d %d %d %d", &test_id, &test2_id, &x, &y);
			if (test_id == server_id && test2_id == client_id)
				clientConfirmBlock(x, y);
		}
	}
	else if (!strcmp("[BLKP:", msgtype))
	{
		int test_id;
		int x, y, block, block2;
		sscanf(message, "%d %d %d %d %d", &test_id, &x, &y, &block, &block2);
		if (test_id == server_id)
		{
			//This Game
			//Set The Block
			recievePlaceBlock(x, y, block, block2);
		}
	}
	else if (!strcmp("[BLKI:", msgtype))
	{
		int test_id, test2_id;
		int x, y, block, block2;
		sscanf(message, "%d %d %d %d %d %d", &test_id, &test2_id, &x, &y, &block, &block2);
		if (test_id == server_id)
		{
			//This Game
			//Set The Block
			matchBlocksHost(test2_id, x, y, block, block2);
		}
	}
	else if (!strcmp("[BLKC:", msgtype))
	{
		if (host == false)
		{
			int test_id, test2_id;
			int x, y, block, block2;
			sscanf(message, "%d %d %d %d %d %d", &test_id, &test2_id, &x, &y, &block, &block2);
			if (test_id == server_id && test2_id == client_id)
			{
				//This Game
				//Set The Block
				matchBlocks(x, y, block, block2);
			}
		}
	}
	else if (!strcmp("[BR:", msgtype))
	{
		if (host == true)
		{
			int test_id;
			int test2_id;
			int x, y;
			sscanf(message, "%d %d %d %d", &test_id, &test2_id, &x, &y);
			client_id = test2_id;
			if (test_id == server_id)
			{
				//Respond, we are that server.
				sendblock(test2_id, x, y);
			}
		}
	}
	else if (!strcmp("[B:", msgtype))
	{
		if (host == false)
		{
			int test_id;
			int test2_id;
			int x, y;
			int block_id, bgblock_id;
			int amount;
			sscanf(message, "%d %d %d %d %d %d %d", &test_id, &test2_id, &x, &y, &block_id, &bgblock_id, &amount);
			if (test_id == server_id && test2_id == client_id) setBlock(x, y, block_id, bgblock_id, amount);
		}
	}
	else if (!strcmp("[MOB:", msgtype))
	{
		int test_id;
		int a, b, c, d, e, f;
		sscanf(message, "%d %d %d %d %d %d %d", &test_id, &a, &b, &c, &d, &e, &f);
		printmessage = false;
	}
	else if (!strcmp("[DIE:", msgtype))
	{
		int test_id;
		int a;
		sscanf(message, "%d %d", &test_id, &a);
		//if (test_id == server_id) killMob(a);
	}
	if (printmessage) printf("\x1b[0;0Hmsgtype %s message %s                                        \n", msgtype, message);
}

void nifiConfirmBlocksAllPlayers(int x, int y)
{
	int i;
	for (i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients[i] != 0)
		{
			//Send confirm to every client
			sprintf((char *) buffer, "[CFMB: %d %d %d %d", server_id, clients[i], x, y);
			Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
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
	Wifi_InitDefault(false);
	Wifi_SetPromiscuousMode(1);
	Wifi_RawSetPacketHandler(Handler);
	Wifi_SetChannel(10);
	nifiClearClients();
}

void nifiEnable()
{
	Wifi_EnableWifi();
	wifiEnabled = true;
}

void nifiDisable()
{
	Wifi_DisableWifi();
	wifiEnabled = false;
	nifiClearClients();
}

bool isWifi() //Checks if wifi is enabled
{
	return wifiEnabled;
}

bool hostNifiInit()
{
	host = true;
	server_id = rand() % 1024;
	return true;
}

bool clientNifiInit()
{
	host = false;
	if (clientReInit == true)
	{
		client_id = rand() % 1024;
		iprintf("\x1b[11;1HClient ID: %d", client_id);
		lookForServers = true;
		clientReInit = false;
	}
	if (foundServer)
	{
		iprintf("\x1b[10;0HDo you wish to join server\nID: %d ?", server_id);
		drawButton(2, 15, 5);
		drawButton(15, 15, 5);
		printXY(4, 16, "Yes");
		printXY(17, 16, "No");
		bool something = true;
		touchPosition touch;
		while (something)
		{
			vBlank();
			scanKeys();
			touchRead(&touch);
			if (keysDown() & KEY_TOUCH && touch.px < 100)
			{
				printXY(4, 16, "   ");
				printXY(17, 16, "  ");
				int i, j;
				printXY(0, 10, "\x1b[2K\n\x1b[2K");
				for (i = 0; i <= 30; ++i)
					for (j = 14; j <= 18; ++j)
						setSubBgTile(i, j, 0);
				something = false;
				return true;
			}
			else if (keysDown() & KEY_TOUCH)
			{
				printXY(4, 16, "   ");
				printXY(17, 16, "  ");
				int i, j;
				printXY(0, 10, "\x1b[2K\n\x1b[2K");
				for (i = 0; i <= 30; ++i)
					for (j = 14; j <= 18; ++j)
						setSubBgTile(i, j, 0);
				something = false;
				foundServer = false;
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
	++frame;
	if (host)
	{
		if (frame >= 600 + (rand() % 300))
		{
			sprintf((char *) buffer, "[PING: %d", server_id);
			Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
			frame = 0;
		}
	}
}
