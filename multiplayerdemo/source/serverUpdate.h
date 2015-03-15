int framcount=0;
void serverOpen(){
	//iprintf("Sending #Sseed\n");
	sprintf((char *)buffer,"#S%c", serverseed);
	if(Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer) != 0)
		iprintf("ERROR\n");
	framcount=0;
}
void serverUpdate(){
	framcount++;
	if (framcount==120 && !client) serverOpen();
	if (client) framcount=0;
}
int pix=0;
void Handler(int packetID, int readlength)
{
	static char data[4096];
	static int bytesRead;
	bytesRead = Wifi_RxRawReadPacket(packetID, readlength, (unsigned short *)data);
	//Check to see if A server is available
	if (data[32]=='#' && data[33]=='S'){
		if (newserver==0){
			newserver=1;
			iprintf("New Server Available %d\nPress A to join\nPress B to discard\n",data[34]);	
			serverseed=data[34];
		}
	}//Asked for a pixel dump
	if (data[32]=='#' && data[33]=='P' && data[34]==serverseed && !client){
			int selected=rand()% maxColours+1;
			sprintf((char *)buffer,"!P%c%c", serverseed,selected);
			if(Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer) != 0)
				iprintf("ERROR\n");
			for (pix=0;pix<=256*192;pix++)
				VRAM_A[pix]=colours[selected];
		
	}//Responce to a pixel dump
	if (data[32]=='!' && data[33]=='P' && data[34]==serverseed){
		for (pix=0;pix<=256*192;pix++)
			VRAM_A[pix]=colours[data[35]];

	}	
	if (data[32]=='!' && data[33]=='L' && data[34]==serverseed){
		DrawLine(data[35], data[36], data[37], data[38], colours[data[39]]);	
	}
}
