#pragma once

#include "Graphics.hpp"
#include "../Block.hpp"
#include "../utils.hpp"
#include "../SmoothCoord.hpp"


class MainRenderer {
public:
    explicit MainRenderer(Graphics &graphics);
    void renderBlock(cVec2i &pos, Block block, int brightness);
    void begin();
    void update();
    void updateCenter(const Vec2f &pos);
    void setCenter(const Vec2f &pos);

    const Vec2f &getCam();
    Vec2i project(cVec2f &pos);

private:
    void setScroll(cVec2i &pos);
    inline void setTileXY(cVec2i &pos, int tile, int palette);
    void renderTile16(Vec2i pos, int tile, int palette);

    int mapId;
    u16 *tileMap;
    SmoothCoord cam{{0, 0}, 0.08};
    Vec2f camRender;  // Rounded render position of the cam for the current frae
};
