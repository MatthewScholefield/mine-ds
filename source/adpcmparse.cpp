#include <nds.h>
#include "audio.hpp"

int imaStepTable[89] = {
        7, 8, 9, 10, 11, 12, 13, 14,
        16, 17, 19, 21, 23, 25, 28, 31,
        34, 37, 41, 45, 50, 55, 60, 66,
        73, 80, 88, 97, 107, 118, 130, 143,
        157, 173, 190, 209, 230, 253, 279, 307,
        337, 371, 408, 449, 494, 544, 598, 658,
        724, 796, 876, 963, 1060, 1166, 1282, 1411,
        1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
        3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
        7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
        15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
        32767
};
int imaIndexTable[16] = {
        -1, -1, -1, -1, 2, 4, 6, 8,
        -1, -1, -1, -1, 2, 4, 6, 8
};
bool get;
char n;
int bptr = 0;

char getCharacter(FILE *f, WaveInfo *w) {
    if (bptr >= WAV_BUFFER_SIZE) {
        fread(w->buffer, 1, WAV_BUFFER_SIZE, f);
        bptr = 0;
    }
    return w->buffer[bptr++];
}

int getNibble(FILE *f, WaveInfo *w) {
    if (get) {
        n = getCharacter(f, w);
        get = false;
        return n & 0x0f;

    } else {
        get = true;
        return (n & 0xF0) >> 4;
    }
}

int predictor = 0;
int stepIndex = 0;
int step = 0;
int reset = 0;

void ADCMReset() {
    predictor = 0;
    stepIndex = 0;
    step = 0;
    reset = 0;
    get = true;
    n = 0;
    bptr = 0;
}

s16 getADCM(FILE *f, WaveInfo *w) {
    if (reset == 0) {
        reset = w->blockAlign * 2 - 8;
        predictor = 0;
        getCharacter(f, w);
        getCharacter(f, w);
        stepIndex = getCharacter(f, w);
        step = 0;
        getCharacter(f, w);
    } else {
        int nibble = getNibble(f, w);
        step = imaStepTable[stepIndex];
        int diff = (step >> 3) + ((nibble & 7) * (step >> 2));
        if (nibble & 8) predictor -= diff;
        else predictor += diff;
        if (predictor > 32767) predictor = 32767;
        else if (predictor < -32768) predictor = -32768;
        stepIndex += imaIndexTable[nibble & 7];
        if (stepIndex > 88) stepIndex = 88;
        if (stepIndex < 0) stepIndex = 0;
        reset -= 1;
    }
    return (s16) predictor;
}
