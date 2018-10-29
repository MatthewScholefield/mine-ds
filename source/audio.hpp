#pragma once

#include <nds.h>
#include <cstdio>

#define WAV_BUFFER_SIZE 4096
typedef struct {
    unsigned int sampleRate;
    unsigned int blockAlign;
    unsigned int dataSize;
    char *buffer;
} WaveInfo;

extern "C"
{
void ADCMReset();
int parseWave(FILE *f, WaveInfo *w);
s16 getADCM(FILE *f, WaveInfo *w);
char getCharacter(FILE *f, WaveInfo *w);
}
