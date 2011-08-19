#include <nds.h>
#include <stdio.h>
#include "scene.h"
#include "startscreen.h"
#include "Background.h"
#include "Font.h"
#include "exitscene.h"
exitscene::exitscene(){

}
void exitscene::start(){
	videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);
	int x;
	for (x=0;x<=256*192;x++)
		VRAM_A[x]=0;
	videoSetModeSub(MODE_5_2D);
	Background bg2 = Background(6,0);
	bg2.print("Press Start to exit",tahoma11,RGB15(31,31,31),50,50);
}
void exitscene::update(){
	scanKeys();
	if (keysDown() & KEY_START){
		Scene* sc = new startscreen;
		this->ChangeScene(sc);
	}
}
