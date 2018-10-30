#pragma once

#include <nds/ndstypes.h>

#define V_FLIP 2
#define H_FLIP 1
#define BOTH_FLIP 3


class TitleFontSystem;
class GraphicsSystem;


class SubRenderer {
public:
    explicit SubRenderer(TitleFontSystem &fontSystem, GraphicsSystem &graphics);
    inline void setSubTileXY(int x, int y, int tile, int palette, int flip);
    void setSubBgTile(int x, int y, int tile);
    void setSubBgTile(int x, int y, int tile, int flip);
    void drawBackground(bool firstSlot = true, bool mineDS = true);
    int getSubBgID();
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
    TitleFontSystem &getTitleFont();

private:
    void drawBackOffset(int offX, int offY, bool mineDS = true);

    TitleFontSystem &fontSystem;
    GraphicsSystem &graphics;
    int subBgID = 6;
    uint16 *tileMap;
    double subBgCalcX = 0;
    double subBgCalcY = 0;
    int subBgX = 0;
    int subBgY = 0;
};
