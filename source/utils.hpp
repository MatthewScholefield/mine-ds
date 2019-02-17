#pragma once

#include <cstdio>
#include <nds.h>
#include <vector>
#include <string>
#include <cmath>
#include "linalg.hpp"

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

#define VERSION_STRING "v0.4.1"

using uint = unsigned int;
template<typename T, size_t N>
using Vec = linalg::vec<T, N>;
template<typename T>
using Vec2 = Vec<T, 2>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;
using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned int>;

using cVec2f = const Vec2f;
using cVec2d = const Vec2d;
using cVec2i = const Vec2i;
using cVec2u = const Vec2u;

void sleepThread(float seconds);
void vBlank();


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
