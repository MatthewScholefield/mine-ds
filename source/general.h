#pragma once
#include <stdio.h>
#include <nds.h>
#include <vector>
#include <string>

#define SHOW_ERRORS

#define FPS		(60)
#define SEC_TO_FPS(S)	((S) * FPS)
#define PI 3.14159265
#define PROFILE_TIME(a)         \
if (keysDown() & KEY_B)         \
{                               \
	cpuStartTiming(0);          \
	a;                          \
	int time = cpuEndTiming();  \
	printXY(1,1,time);          \
}                               \
else                            \
	a;

void sleepThread(unsigned int seconds);
void printGlobalMessage(const char* s);
#define MESSAGE_COUNT 3
#define MESSAGE_CLEAR_DELAY 512
#define VERSION_STRING "v0.3.1"

void update_message();
void printLocalMessage(const char* s);
void clear_messages();
inline void printXY(int x, int y, const char *output);
inline void printXY(int x, int y, int output);
void timeUpdate();
unsigned int getTime();
void vBlank();
size_t maxStringLength(std::vector<std::string> lines);
void clearText(bool firstSection, int sizeY = 64);
void clearText();

inline void printXY(int x, int y, const char *output)
{
	//if (y > 31 || x > 31) return;
	printf("\x1b[%d;%dH%s", y, x, output);
}

inline void printXY(int x, int y, int output)
{
	//if (y > 31 || x > 31) return;
	printf("\x1b[%d;%dH%d", y, x, output);
}

template<typename T> inline const T abs(T const & x)
{
	return ( x < 0) ? -x : x;
}

inline void showError(const char* error)
{
#ifdef SHOW_ERRORS
	for (int i = 0; i < 24; ++i)
		printXY(0, i, error);
	while (1)
		swiWaitForVBlank();
#endif
}
