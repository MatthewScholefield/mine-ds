#pragma once

#include "Block.hpp"
#include "SmoothCoord.hpp"

class MainRenderer;


class World {
public:
    static constexpr int sx = 256, sy = 64, blockSize = 16;

    void generate();
    void update();
    void render(MainRenderer &renderer);

private:
    Block blocks[sx][sy]{};
    SmoothCoord cam{0, 0};
};
