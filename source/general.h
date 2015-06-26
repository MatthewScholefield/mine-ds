#pragma once
#include <stdio.h>
#include <vector>
#include <string>

#define FPS		(60)
#define SEC_TO_FPS(S)	((S) * FPS)
#define PI 3.14159265

void sleepThread(unsigned int seconds);
void printGlobalMessage(const char* s);
#define MESSAGE_COUNT 3
#define MESSAGE_CLEAR_DELAY 500
#define VERSION_STRING "0.2.0"

void update_message();
void printLocalMessage(const char* s);
void clear_messages();
inline void printXY(int x, int y, const char *output);
inline void printXY(int x, int y, int output);
void updateTime();
unsigned int getTime();
void updateFrame();
size_t maxStringLength(std::vector<std::string> lines);
void clearText(bool firstSection, int sizeY = 64);
void clearText();

inline void printXY(int x, int y, const char *output) {
	iprintf("\x1b[%d;%dH%s", y, x, output);
}

inline void printXY(int x, int y, int output) {
	iprintf("\x1b[%d;%dH%d", y, x, output);
}

template<typename T> inline const T abs(T const & x) {
	return ( x < 0) ? -x : x;
}