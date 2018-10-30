#pragma once

#include "GraphicsSystem.hpp"

class MainRenderer {
public:
    explicit MainRenderer(GraphicsSystem &graphics);
    void renderBlock(int i, int j, int blockId);

private:
    inline void setTileXY(int x, int y, int tile, int palette);
    void renderTile16(int x, int y, int tile, int palette);

    u16 *tileMap;
};
