#include <nds.h>
#include <dswifi9.h>
#include <stdio.h>
unsigned short buffer[100];
unsigned short buffer2[100];
bool playerjoined[10];
int num;
int player;
int seed;
void Handler(int packetID, int readlength)
{
	static char data[4096];
	static int bytesRead;
	bytesRead = Wifi_RxRawReadPacket(packetID, readlength, (unsigned short *)data);
	//Check to see if somebody joined
	//This Code is NOT in anyway needed.
	if (data[32]=='!' && data[33]=='!'){
		if (!data[32]==seed)
			playerjoined[data[34]]=true; //Just set a bool saying that another player has joined...
	}
	//End of NOT needed code...
	
	//Other wise a message was sent.
	if (data[32]=='*' && data[33]=='*'){
		iprintf("Message from %d:%d",data[34],data[35]);	
	}
}
void NiFiInit(){
	swiWaitForVBlank();
	Wifi_InitDefault(false);
	Wifi_SetPromiscuousMode(1);
	Wifi_EnableWifi();
	Wifi_RawSetPacketHandler(Handler);
	Wifi_SetChannel(10);
	iprintf("NiFi Inited\n");
}
int main(){
	consoleDemoInit();
	iprintf("NiFi Demo\nNow starting NiFi!\n");
	NiFiInit();
	int i;
	for (i=0;i<=9;i++) playerjoined[i]=false;

	while(1){
		consoleClear();
		iprintf("Please choose a player number\n");
		iprintf("It must be different to the \nother DS around you!\n");
		iprintf("Press UP for number UP and DOWN for\nnumber DOWN\nPress Start to confirm\n");
		scanKeys();
		if (keysDown() & KEY_UP) seed++;
		else if (keysDown() & KEY_DOWN) seed--;
		if (seed>9) seed=9;
		else if (seed<0) seed=0;
		iprintf("\n\nPlayerNumber: %d",seed);
		if (keysDown() & KEY_START) break; //When you press start Break out of the while loop regardless of whether the thing in the () is equal to 1.
		swiWaitForVBlank();
	}	
	bool conected;
	int fram=0;
	sprintf((char *)buffer,"!!%d", seed);
	iprintf("Finished Setup, ready to send!\n");
	while(1){
		fram++;
		//This code is NOT in anyway needed...
		if (fram==5)//Every 5 Frames send a connected message
			if(Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer) != 0)
				iprintf("ERROR\n");
		//End of NOT needed code...
		scanKeys();
		if (keysDown() & KEY_A){
			num=1;
			//Print the seed and number into a buffer
			//The buffer starts with ** which tells the listening DS that this is a Message
			sprintf((char *)buffer2,"**%d%d", seed,num);
			//This sends the buffer2 to other DS's around you.
			if(Wifi_RawTxFrame(strlen((char *)buffer2) + 1, 0x0014, buffer2) != 0)
				iprintf("ERROR\n");
		}
		swiWaitForVBlank();
	}
}
