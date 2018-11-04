#pragma once

#include <nds/ndstypes.h>
#include "../SmoothCoord.hpp"

enum class Flip : int {
    None = 0,
    Horiz = 1,
    Vert = 2,
    Both = 3
};


class Font;
class Graphics;


class SubRenderer {
public:
    static constexpr int tileSx = 32, tileSy = 24;
    static constexpr int mapPx = 512, mapPy = 512;
    explicit SubRenderer(Graphics &graphics);
    void setTile(int x, int y, int tile);
    void setTile(int x, int y, int tile, Flip flip);
    void drawBackground(bool firstSlot = true, bool mineDS = true);
    void move(const Vec2f &delta);
    void set(const Vec2f &pos);
    void update();

    void drawButton(int x, int y, int sizex);
    void drawButtonColored(int x, int y, int sizex);
    void drawBoxCenter(int x, int y, int lx, int ly);
    void drawBoxFrame(int x, int y, int lx, int ly);
    void drawBox(int x, int y, int lx, int ly);

    int getScrollX();
    int getScrollY();

private:
    inline void setTileXY(int x, int y, int tile, int palette, Flip flip);
    void drawBackOffset(int offX, int offY, bool mineDS = true);

    Graphics &graphics;
    int mapId;
    uint16 *tileMap;
    int consoleId;
    SmoothCoord pos{{0, 0}, 0.08f};
};
