#include <nds.h>
#include <stdio.h>
#include "audio.h"
int ima_step_table[89] = { 
		7,	  8,	9,	 10,   11,	 12,   13,	 14,
	   16,	 17,   19,	 21,   23,	 25,   28,	 31,
	   34,	 37,   41,	 45,   50,	 55,   60,	 66,
	   73,	 80,   88,	 97,  107,	118,  130,	143,
	  157,	173,  190,	209,  230,	253,  279,	307,
	  337,	371,  408,	449,  494,	544,  598,	658,
	  724,	796,  876,	963, 1060, 1166, 1282, 1411,
	 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
	 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
	 7132, 7845, 8630, 9493,10442,11487,12635,13899,
	15289,16818,18500,20350,22385,24623,27086,29794,
	32767
};
int ima_index_table[16] = {
  -1, -1, -1, -1, 2, 4, 6, 8,
  -1, -1, -1, -1, 2, 4, 6, 8
}; 
bool get;
char n;
int getNibble(FILE* f)
{
  if (get)
  {
    n = fgetc(f);
    get = false;
    return n & 0x0f;

  }
  else
  {
    get = true;
    return (n & 0xF0) >> 4;
  }
}
int predictor = 0;
int step_index = 0;
int step = 0;
int reset = 0;
void ADCMReset()
{
  predictor = 0;
  step_index = 0;
  step = 0;
  reset = 0;
  get = true;
  n = 0;
}
s16 getADCM(FILE* f,waveInfo* w)
{
  int nibble = 0;
  if (reset == 0 )
  {
    reset = w->blockAlign*2;
    fgetc(f);
    fgetc(f);
    predictor = 0;
    step_index = fgetc(f);
    step = 0;     
    fgetc(f);
    putchar(predictor&0xff);
    putchar((predictor&0xFF00)>>8);
    reset-=8;
  }
  else 
  {
    nibble = getNibble(f);
    step = ima_step_table[step_index];
    int diff = step >> 3;    
  	if(nibble&4)
  		diff += step;
  	if(nibble&2)
  		diff += step>>1;
  	if(nibble&1)
  		diff += step>>2;
    if (nibble&8) predictor -= diff;
    else predictor += diff ;
    if (predictor > 32767) predictor = 32767;
    else if (predictor < -32768) predictor = -32768;
    step_index += ima_index_table[nibble&7];
    if (step_index > 88) step_index = 88;
    if (step_index < 0) step_index = 0;
    reset-=1;
 }
 return (s16)predictor;
}
