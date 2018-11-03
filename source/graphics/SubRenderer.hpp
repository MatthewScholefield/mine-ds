#pragma once

#include <nds/ndstypes.h>

#define V_FLIP 2
#define H_FLIP 1
#define BOTH_FLIP 3


class Font;
class Graphics;


class SubRenderer {
public:
    explicit SubRenderer(Graphics &graphics);
    void setTile(int x, int y, int tile);
    void setTile(int x, int y, int tile, int flip);
    void drawBackground(bool firstSlot = true, bool mineDS = true);
    void move(int dx, int dy);
    void set(int x, int y);
    void update();

    void drawButton(int x, int y, int sizex);
    void drawButtonColored(int x, int y, int sizex);
    void drawBoxCenter(int x, int y, int lx, int ly);
    void drawBoxFrame(int x, int y, int lx, int ly);
    void drawBox(int x, int y, int lx, int ly);

    int getScrollX();
    int getScrollY();

private:
    inline void setTileXY(int x, int y, int tile, int palette, int flip);
    void drawBackOffset(int offX, int offY, bool mineDS = true);

    Graphics &graphics;
    int mapId;
    uint16 *tileMap;
    int consoleId;
    double subBgCalcX = 0, subBgCalcY = 0;
    int subBgX = 0, subBgY = 0;
};
