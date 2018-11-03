#pragma once

#include "Graphics.hpp"
#include "../Block.hpp"


class MainRenderer {
public:
    explicit MainRenderer(Graphics &graphics);
    void renderBlock(int i, int j, Block block);
    void setScroll(int x, int y);

private:
    inline void setTileXY(int x, int y, int tile, int palette);
    void renderTile16(int x, int y, int tile, int palette);

    int mapId;
    u16 *tileMap;
};
