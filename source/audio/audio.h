#pragma once
#include <nds.h>
#include <stdio.h>

typedef struct
{
	int sampleRate;
	int blockAlign;
	int dataSize;
} WaveInfo;

int parseWave(FILE* f, WaveInfo* w);
s16 getADCM(FILE* f, WaveInfo* w);
void ADCMReset();