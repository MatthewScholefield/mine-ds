#include <nds.h>
#include <stdio.h>
#include "ndsvideo.h" // Include the video functions (for setupVideo();)
#include "blockID.h" //Want to use it to display the current block
#include "world.h"
#include "top-screen.h"
#include "subscreen.h" //image file

void subBGSetup(){ //Its a setup function, not a update function :P

	setupVideo(); //Setup all the video we need (in ndsvideo.h/cpp)

    int bg = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

    dmaCopy(subscreenBitmap, bgGetGfxPtr(bg), 256*256);
    dmaCopy(subscreenPal, BG_PALETTE_SUB, 256*2);
}
