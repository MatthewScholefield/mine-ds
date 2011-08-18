#include <nds.h>
#include <stdio.h>
#include "scene.h"
#include "startscreen.h"
#include "exitscene.h"
exitscene::exitscene(){

}
void exitscene::start(){
	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);
	int x;
	for (x=0;x<=256*192;x++)
		VRAM_A[x]=0;
	videoSetModeSub(MODE_0_2D);
   	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	consoleDemoInit();
	iprintf("Press Start to return to the startscene\n");
}
void exitscene::update(){
	scanKeys();
	if (keysDown() & KEY_START){
		Scene* sc = new startscreen;
		this->ChangeScene(sc);
	}
}
