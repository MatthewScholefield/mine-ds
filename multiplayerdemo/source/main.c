#include <nds.h>
#include <dswifi9.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
bool client;
unsigned short buffer[100];
int serverseed=0;
int newserver;
#include "lines.h"
#include "colours.h"
#include "serverUpdate.h"
#include "NiFiInit.h"
int main(){
	srand(time(NULL));
	int seed1=rand();
	int colour=0;
	consoleDemoInit();
	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);
	lcdMainOnBottom();
	NiFiInit();
	newserver=3;
	touchPosition touch;
	int oldX = 0;
	int oldY = 0;
	printf("Press Start for a client\nSelect for a server\n");
	while(1){
		scanKeys();
		if (keysDown() & KEY_START){
			client=true;
			break;
		}
		else if (keysDown() & KEY_SELECT){
			client=false;
			break;
		}
		swiWaitForVBlank();
	}
	int seed2=rand();
	if (!client){
		serverseed=(seed1&seed2)&0xFF;
		serverseed+=serverseed==0; //If the server seed is 0 change to 1

	}
	printf("Waiting for connections\n");
		int i;
	for (i=0;i<=256*192;i++)
		VRAM_A[i]=colours[rand()%maxColours];
	newserver=0;
	if (client){
		while(newserver!=2){
			scanKeys();
			if (newserver==1){
				if (keysDown() & KEY_A) newserver=2;
				else if (keysDown() & KEY_B) newserver=0;
			}		
		}
		//Now Ask for a Pixel dump
		sprintf((char *)buffer,"#P%c", serverseed);
			if(Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer) != 0)
				iprintf("ERROR\n");
	}
	consoleClear();
	printf("ServerID: %d\n",serverseed);
	iprintf("\nWelcome to DSPAINT Multiplayer!\nPress Y to see what mode you are in\nPress START to change the mode\nPress UP and DOWN to change colour\nPress X to fill the screen with the selected colour\nTap the screen to draw!\n\n\n\nHave Fun!\n");
	while(1){
		scanKeys();
		touchRead(&touch);
		serverUpdate();
		if (keysDown() & KEY_X){
			sprintf((char *)buffer,"!P%c%c", serverseed,colour);
			if(Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer) != 0)
				iprintf("ERROR\n");
			for (pix=0;pix<=256*192;pix++)
				VRAM_A[pix]=colours[colour];
		}
		if (keysDown() & KEY_Y){
			if (client) iprintf("Server is currently off\nPress START to change\nTurning the server on could\ncause too many packets to be sent\nbe careful!\n");
			else iprintf("Server is currently on\nPress START to change\n");
		}
		if (keysDown() & KEY_START)
			client=!client;
		if (keysDown() & KEY_UP) colour++;
		else if (keysDown() & KEY_DOWN) colour--;
		if (colour<0) colour=0;
		if (colour==maxColours+1)colour=maxColours;
		if (keysDown()) printf("Colour:%d,%s\n",colour,colournames[colour]);
		if (!(keysDown() & KEY_TOUCH) && (keysHeld() & KEY_TOUCH)){
			sprintf((char *)buffer,"!L%c%c%c%c%c%c", serverseed,oldX,oldY,touch.px,touch.py,colour);
			if(Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer) != 0)
				iprintf("ERROR\n");	
			else DrawLine(oldX, oldY, touch.px, touch.py, colours[colour]);			
		}	
		oldX = touch.px;
		oldY = touch.py;
		swiWaitForVBlank();		
	}
}
		
