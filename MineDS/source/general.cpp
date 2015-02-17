#include <stdio.h>
#include <string>
#include <nds.h>
#include <dswifi9.h>
#include "nifi.h"
#include "general.h"

std::string messages[3];
int nextmsg = 0;
int currentTime = 0;

void sleep(unsigned int seconds)
{
	for (unsigned int i = 0; i < SEC_TO_FPS(seconds); ++i)
		swiWaitForVBlank(); // sleeps for one frame
}

void printLocalMessage(const char* s)
{
	messages[nextmsg] = s;
	++nextmsg;
	if (nextmsg >= 3) nextmsg = 0;
}

void printGlobalMessage(const char* s)
{
	messages[nextmsg] = s;
	++nextmsg;
	if (nextmsg >= 3) nextmsg = 0;
	if (isWifi())
	{
		unsigned short buffer[100];
		int server_id = getServerID();
		sprintf((char *) buffer, "[MSG: %d %s", server_id, s);
		Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
	}
}

void update_message()
{
	for (int i = 0; i < 3; ++i)
		iprintf("\x1b[%d;0H\x1b[2K", 20 + i);
	int start = (nextmsg - 1) % 3;
	if (start == -1) start = 2;
	for (int amount = 0; amount < 3; ++amount)
	{
		iprintf("\x1b[%d;0H %s", 20 + amount, messages[start].c_str());
		--start;
		if (start == -1) start = 2;
	}
}

void clear_messages() //Adds 3 lines of empty messages
{
	printGlobalMessage(" ");
	printGlobalMessage(" ");
	printGlobalMessage(" ");
	update_message();
}

// TODO: Make updateTime() and getTime() less misleading,
//       since they don't actually get the time ( gettimeofday() )

void updateTime()
{
	++currentTime;
	if (currentTime > 10000)
		currentTime = 1;
}

unsigned int getTime()
{
	return currentTime;
}
