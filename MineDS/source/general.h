#pragma once
#include <stdio.h>

#define FPS		(60)
#define SEC_TO_FPS(S)	((S) * FPS)

void sleep(unsigned int seconds);
void printGlobalMessage(const char* s);
void update_message();
void printLocalMessage(const char* s);
void clear_messages();
inline void printXY(int x, int y, const char *output);
inline void printXY(int x, int y, int output);
void updateTime();
unsigned int getTime();


inline void printXY(int x, int y, const char *output)
{
	iprintf("\x1b[%d;%dH%s", y, x, output);
}

inline void printXY(int x, int y, int output)
{
	iprintf("\x1b[%d;%dH%d", y, x, output);
}