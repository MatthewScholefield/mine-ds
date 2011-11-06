#include <nds.h>
u32 keysH;
u32 keysD;
touchPosition touch;
void controlsFrame(){
	scanKeys();
	keysH=keysHeld();
	keysD=keysDown();
	touchRead(&touch);
}
u32 controlsKeysH(){return keysH;}
u32 controlsKeysD(){return keysD;}
touchPosition controlsTouch(){return touch;}

