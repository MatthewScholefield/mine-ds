#include <nds.h>
#include "scene.h"
#include "exitscene.h"
#include "startscreen.h"
#include "startscreenbg.h"
#include "homebottom.h"
startscreen::startscreen(){


}
void startscreen::start(){
        videoSetMode(MODE_5_2D);
   	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

        int bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

        dmaCopy(startscreenbgBitmap, bgGetGfxPtr(bg), 256*256);
        dmaCopy(startscreenbgPal, BG_PALETTE, 256*2);
        
        videoSetModeSub(MODE_5_2D);
   	vramSetBankC(VRAM_C_SUB_BG_0x06200000);

        int bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

        dmaCopy(homebottomBitmap, bgGetGfxPtr(bg3), 256*256);
        dmaCopy(homebottomPal, BG_PALETTE_SUB, 256*2);
}
void startscreen::update(){
	scanKeys();
	if (keysDown() & KEY_START){ 
		consoleDemoInit();
		Scene* sc = new exitscene;
		ChangeScene(sc);
	}
}
