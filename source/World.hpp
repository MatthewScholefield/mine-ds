#pragma once

#include "Block.hpp"
#include "SmoothCoord.hpp"
#include "utils.hpp"

class MainRenderer;


class World {
public:
    static constexpr int sx = 256, sy = 64, blockSize = 16;

    void generate();
    void update();
    void render(MainRenderer &renderer);
    void setCam(const Vec2f &pos);

private:
    Block blocks[sx][sy]{};
    SmoothCoord cam{0, 0};
};
