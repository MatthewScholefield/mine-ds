#include <nds.h>
#include <stdio.h>
typedef struct
{
  int sampleRate;
  int blockAlign;
}waveInfo;

int parseWave(FILE* f,waveInfo* w);
s16 getADCM(FILE* f,waveInfo* w);
