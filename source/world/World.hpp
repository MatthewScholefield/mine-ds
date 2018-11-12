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

    Block &operator[](const Vec2i &pos) {
        return blocks[pos.x][pos.y];
    }

    const Block &operator[](const Vec2i &pos) const {
        return blocks[pos.x][pos.y];
    }

    Block blocks[sx][sy]{};
    const Vec2f gravity{0, 9.81f};
};
