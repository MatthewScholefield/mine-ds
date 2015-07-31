#pragma once
#include <nds.h>
#include <stdio.h>
#define WAV_BUFFER_SIZE 4096
typedef struct
{
	int sampleRate;
	int blockAlign;
	int dataSize;
	char* buffer;
} WaveInfo;
extern "C"
{
void ADCMReset();
int parseWave(FILE* f, WaveInfo* w);
s16 getADCM(FILE* f, WaveInfo* w);
char getCharacter(FILE* f, WaveInfo* w);
}
