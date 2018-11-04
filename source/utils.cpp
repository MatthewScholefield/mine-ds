#include <cstdio>
#include <string>
#include <vector>
#include "utils.hpp"

void vBlank() {
    swiWaitForVBlank();
}

void sleepThread(float seconds) {
    for (unsigned int i = 0; i < SEC_TO_FPS(seconds); ++i) {
        vBlank(); // sleeps for one frame
    }
}
