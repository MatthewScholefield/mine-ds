#pragma once

#include "../Block.hpp"
#include "../SmoothCoord.hpp"
#include "../utils.hpp"

class MainRenderer;

class World {
public:
    static constexpr int sx = 256/8, sy = 64;

    void generate();
    void update();
    void render(MainRenderer &renderer);

    void brightnessUpdate(int x, int y, int brightness);
    void checkBlock(int x, int y);
    void calculateBrightness();
    void calculateBrightness(int x, int y);
    void calculateBrightness(int leftBound, int rightBound, int topBound, int bottomBound);
    void calcBrightnessDefault(int leftBound, int rightBound, int topBound, int bottomBound);
    void calcBrightnessModified(int leftBound, int rightBound, int topBound, int bottomBound);

    Block &operator[](const Vec2i &pos) {
        return fg[pos.x][pos.y];
    }

    Block fg[sx][sy]{}, bg[sx][sy]{};
    int brightness[sx][sy]{};
    int sunBrightness = 15;
    const Vec2f gravity{0, 9.81f};
};
