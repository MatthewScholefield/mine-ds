#pragma once

#include <cstdio>
#include <nds.h>
#include <vector>
#include <string>
#include <cmath>

#define SHOW_ERRORS

#define LENGTH(X)    (sizeof(X) / sizeof((X)[0]))
#define FPS        (60)
#define SEC_TO_FPS(S)    ((S) * FPS)
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
    (a);

using uint = unsigned int;
void sleepThread(unsigned int seconds);

#define MESSAGE_CLEAR_DELAY 512
#define VERSION_STRING "v0.4.1"

void updateMessages();
void printLocalMessage(const char *s);
void clearMessages();
void timeUpdate();
unsigned int getTime();
void vBlank();
size_t maxStringLength(std::vector<std::string> lines);
void clearText(bool firstSection, int sizeY = 64);
void clearText();

using uint = unsigned int;

template<typename...Args>
inline void printXY(int x, int y, const std::string &format, Args...args) {
    printf(("\x1b[%d;%dH" + format).c_str(), y, x, args...);
}

inline void printXY(int x, int y, const std::string &&format) {
    printf(("\x1b[%d;%dH" + format).c_str(), y, x);
}

template<typename T>
inline void printXY(int x, int y, const T &&t) {
    printf(("\x1b[%d;%dH" + std::to_string(t)).c_str(), y, x);
}

template<typename T>
inline const T abs(T const &x) {
    return (x < 0) ? -x : x;
}

template<typename T>
inline const T max(T const &a, T const &b) {
    return a > b ? a : b;
}

template<typename T>
inline const T max(T const &a, T const &b, T const &c) {
    return max(max(a, b), c);
}

template<typename T>
inline const T min(T const &a, T const &b) {
    return a < b ? a : b;
}

template<typename T>
inline const T min(T const &a, T const &b, T const &c) {
    return min(min(a, b), c);
}

template<typename T>
inline int roundInt(T x) {
    return int(std::round(x));
}

inline void showError(const char *error) {
#ifdef SHOW_ERRORS
    for (int i = 0; i < 24; ++i) {
        printXY(0, i, error);
    }
    while (true) {
        swiWaitForVBlank();
    }
#endif
}
