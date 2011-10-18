#include <nds.h>
#include <stdio.h>
#include "ndsvideo.h" // Include the video functions (for setupVideo();)
#include "mainscreen.h" //image file (Just a test background, later the tiled BG)

void mainBGUpdate(){

	setupVideo(); //Setup all the video we need (in ndsvideo.h/cpp)

    vramSetBankG(VRAM_G_MAIN_BG_0x06000000);

    int bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

    dmaCopy(mainscreenBitmap, bgGetGfxPtr(bg), 256*256);
    dmaCopy(mainscreenPal, BG_PALETTE, 256*2);
}
