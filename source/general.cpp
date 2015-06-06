#include <stdio.h>
#include <string>
#include <nds.h>
#include <dswifi9.h>
#include <maxmod9.h>
#include "nifi.h"
#include "general.h"
#include "mainGame.h"
#include "sounds.h"

std::string messages[MESSAGE_COUNT];
int currentTime = 0;
int triggerTime = 1;

void updateFrame()
{
	if (streamIsOpen())
		mmStreamUpdate();
	swiWaitForVBlank();
}

void sleep(unsigned int seconds)
{
	for (unsigned int i = 0; i < SEC_TO_FPS(seconds); ++i)
		updateFrame(); // sleeps for one frame
}

int getOldestMessageIndex()
{
	for (int i = MESSAGE_COUNT - 1; i >= 0; --i)
		if (messages[i].empty())
			return i == MESSAGE_COUNT - 1 ? i : i + 1;
	return 0;
}

void printLocalMessage(const char* message)
{
	for (int i = 0; i < MESSAGE_COUNT - 1; ++i)
		messages[i] = messages[i + 1];
	messages[MESSAGE_COUNT - 1] = message;
	if (getOldestMessageIndex() == MESSAGE_COUNT - 1)
	{ //the first message should reset the trigger time
		triggerTime = getTime() % MESSAGE_CLEAR_DELAY - 1; //Prevent message instantly dissapearing
		if (triggerTime < 0)
			triggerTime = MESSAGE_CLEAR_DELAY - 1;
	}
}

void printGlobalMessage(const char* message)
{
	printLocalMessage(message);
	if (isWifi())
	{
		unsigned short buffer[100];
		int server_id = getServerID();
		sprintf((char *) buffer, "[MSG: %d %s", server_id, message);
		Wifi_RawTxFrame(strlen((char *) buffer) + 1, 0x0014, buffer);
	}
}

void update_message()
{
	if (getTime() % MESSAGE_CLEAR_DELAY == unsigned(triggerTime))
		messages[getOldestMessageIndex()] = "";
	for (int amount = 0; amount < MESSAGE_COUNT; ++amount)
		iprintf("\x1b[%d;0H\x1b[2K %s", 20 + amount, messages[amount].c_str());
}

void clear_messages()
{
	for (int i = 0; i < MESSAGE_COUNT; ++i)
		messages[i] = "";
	update_message();
}

// TODO: Make updateTime() and getTime() less misleading,
//       since they don't actually get the time ( gettimeofday() )

void updateTime()
{
	++currentTime;
	if (currentTime > 100000)
		currentTime = 1;
}

unsigned int getTime()
{
	return currentTime;
}
