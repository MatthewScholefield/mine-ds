#include <nds.h>
#include <stdio.h>
#include "ndsvideo.h" // Include the video functions (for setupVideo();)
#include "blockID.h" //Want to use it to display the current block
#include "world.h"
#include "top-screen.h"
#include "subscreen.h" //image file

void subBGUpdate(){

	setupVideo(); //Setup all the video we need (in ndsvideo.h/cpp)

    vramSetBankC(VRAM_C_SUB_BG_0x06200000);

    int bg = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

    dmaCopy(subscreenBitmap, bgGetGfxPtr(bg), 256*256);
    dmaCopy(subscreenPal, BG_PALETTE_SUB, 256*2);
}
