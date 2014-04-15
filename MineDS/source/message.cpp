#include <stdio.h>
#include <string>
#include <nds.h>
#include <dswifi9.h>
#include "nifi.h"
std::string messages[4];
int nextmsg = 0;
void show_message(const char* s)
{
	messages[nextmsg] = s;
	nextmsg ++;
	if (nextmsg>3) nextmsg = 0;
}
void print_message(const char* s)
{
	messages[nextmsg] = s;
	nextmsg ++;
	if (nextmsg>3) nextmsg = 0;
	if (isWifi())
	{
		unsigned short buffer[100];
		int server_id = getServerID();	
		sprintf((char *)buffer,"[MSG: %d %s", server_id, s);
		Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);		
	}
}

void update_message()
{
    printf("\x1b[20;0H\x1b[2K");
    for (int i=0;i<3;i++) printf("\n\x1b[2K");
    printf("\x1b[20;0H");
    int start = nextmsg-1;
    if (start == -1) start = 3;
    int amount = 0;
    while (amount!=3)
    {
    	printf(" %s",messages[start].c_str());
	start--;
	if (start==-1) start = 3;
            amount++;
    }
}

void clear_messages() //Adds 3 lines of empty messages
{
	print_message(" ");
	print_message(" ");
	print_message(" ");
	update_message();

}