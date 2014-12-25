#pragma once
#define MAX_CLIENTS	(8)

void nifiInit();
void nifiEnable();
void nifiUpdate();
bool hostNifiInit();
void nifiDisable();
bool clientNifiInit();
int getServerID();
int getClientID();
bool isWifi();
bool isHost();
void nifiConfirmBlocksAllPlayers(int x,int y);
