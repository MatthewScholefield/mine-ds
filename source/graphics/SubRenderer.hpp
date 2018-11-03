#pragma once

#include <nds/ndstypes.h>

#define V_FLIP 2
#define H_FLIP 1
#define BOTH_FLIP 3


class Font;
class Graphics;


class SubRenderer {
public:
    explicit SubRenderer(Font &fontSystem, Graphics &graphics);
    inline void setSubTileXY(int x, int y, int tile, int palette, int flip);
    void setSubBgTile(int x, int y, int tile);
    void setSubBgTile(int x, int y, int tile, int flip);
    void drawBackground(bool firstSlot = true, bool mineDS = true);
    void moveSubBg(int dX, int dY);
    void setSubBg(int x, int y);
    int getScrollX();
    int getScrollY();
    void updateSubBG();
    void drawButton(int x, int y, int sizex);
    void drawButtonColored(int x, int y, int sizex);
    void drawBoxCenter(int x, int y, int lx, int ly);
    void drawBoxFrame(int x, int y, int lx, int ly);
    void drawBox(int x, int y, int lx, int ly);
    Font &getTitleFont();

private:
    void drawBackOffset(int offX, int offY, bool mineDS = true);

    Font &fontSystem;
    Graphics &graphics;
    int mapId;
    uint16 *tileMap;
    double subBgCalcX = 0;
    double subBgCalcY = 0;
    int subBgX = 0;
    int subBgY = 0;
};
