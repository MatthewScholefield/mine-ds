#pragma once

#include "Graphics.hpp"
#include "../Block.hpp"
#include "../utils.hpp"
#include "../SmoothCoord.hpp"


class MainRenderer {
public:
    explicit MainRenderer(Graphics &graphics);
    void renderBlock(int i, int j, Block block);
    void setScroll(int x, int y);
    void update();
    void updateCenter(const Vec2f &pos);

    const Vec2f &getCam();

private:
    inline void setTileXY(int x, int y, int tile, int palette);
    void renderTile16(int x, int y, int tile, int palette);

    int mapId;
    u16 *tileMap;
    SmoothCoord cam{{0, 0}, 0.08};
};
