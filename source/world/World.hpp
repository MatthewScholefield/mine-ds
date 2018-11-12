#pragma once

#include "../Block.hpp"
#include "../SmoothCoord.hpp"
#include "../utils.hpp"

class MainRenderer;

class World {
public:
    static constexpr int sx = 256, sy = 64, blockSize = 16;

    void generate();
    void update();
    void render(MainRenderer &renderer);
    Block blocks[sx][sy]{};
    const Vec2f gravity{0, 9.81f};
};
