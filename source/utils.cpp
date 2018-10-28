#include <cstdio>
#include <string>
#include <vector>
#include "utils.hpp"


unsigned int currentTime = 0;

void vBlank() {
    swiWaitForVBlank();
}

size_t maxStringLength(std::vector<std::string> lines) {
    size_t maxLength = 0;
    for (const auto &line : lines) {
        if (line.length() > maxLength) {
            maxLength = line.length();
        }
    }
    return maxLength;
}

void sleepThread(unsigned int seconds) {
    for (unsigned int i = 0; i < SEC_TO_FPS(seconds); ++i) {
        vBlank(); // sleeps for one frame
    }
}

// TODO: Make updateTime() and getTime() less misleading,
//       since they don't actually get the time ( gettimeofday() )

void timeUpdate() {
    ++currentTime;
    if (currentTime > 100000) {
        currentTime = 1;
    }
}

unsigned int getTime() {
    return currentTime;
}
