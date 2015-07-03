#include <stdio.h>
#include "audio.h"
bool require(char c,FILE* f)
{
  if (fgetc(f)!=c) return true;
  return false;
}
bool findfmt(FILE* f)
{
  while (fgetc(f)!='f'){
    if (feof(stdin)) return true;
  }
  if (fgetc(f)!='m') findfmt(f);
  if (fgetc(f)!='t') findfmt(f);
  if (fgetc(f)!=' ') findfmt(f);
  return false;
}
bool finddata(FILE* f)
{
  while (fgetc(f)!='d'){
    if (feof(stdin)) return true;
  }
  if (fgetc(f)!='a') finddata(f);
  if (fgetc(f)!='t') finddata(f);
  if (fgetc(f)!='a') finddata(f);
  return false;
}
int parseWave(FILE* f,waveInfo* w)
{
  int i;
    //Parse the .wav header
  if (require('R',f)) return -1;
  if (require('I',f)) return -1;
  if (require('F',f)) return -1;
  if (require('F',f)) return -1;
  //Ignore the size
  for (i = 0; i < 4; ++i)
    fgetc(f);
  if (require('W',f)) return -1;
  if (require('A',f)) return -1;
  if (require('V',f)) return -1;
  if (require('E',f)) return -1;
  if (findfmt(f)) return -1;
  //Ignore the size
  for (i = 0; i < 4; ++i)
    fgetc(f);
  if (require(0x11,f)) return -2;
  if (require(0x00,f)) return -2;  
  if (require(0x01,f)) return -3;
  if (require(0x00,f)) return -3;
 
  int samplingRate = 0;
  fread(&samplingRate,4,1,f);
  //Ignore the size 
  for (i = 0; i < 4; ++i)
    fgetc(f);
  int blockSize = fgetc(f);
  blockSize |= fgetc(f)<<8;
  if (finddata(f)) return -4;
  //Ignore the size
  int dataSize = 0;
  dataSize |= fgetc(f);
  dataSize |= fgetc(f)<<8;
  dataSize |= fgetc(f)<<16;
  dataSize |= fgetc(f)<<24;
  w->blockAlign = blockSize;  
  w->sampleRate = samplingRate;
	w->dataSize = dataSize;
	return 0;
}
