void NiFiInit(){
	swiWaitForVBlank();
	Wifi_InitDefault(false);
	Wifi_SetPromiscuousMode(1);
	Wifi_EnableWifi();
	Wifi_RawSetPacketHandler(Handler);
	Wifi_SetChannel(10);
	iprintf("NiFi Inited\n");
}
