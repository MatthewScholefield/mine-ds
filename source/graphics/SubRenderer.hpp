#pragma once

#include <nds/ndstypes.h>

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
    explicit SubRenderer(Graphics &graphics);
    void setTile(int x, int y, int tile);
    void setTile(int x, int y, int tile, Flip flip);
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
    inline void setTileXY(int x, int y, int tile, int palette, Flip flip);
    void drawBackOffset(int offX, int offY, bool mineDS = true);

    Graphics &graphics;
    int mapId;
    uint16 *tileMap;
    int consoleId;
    double subBgCalcX = 0, subBgCalcY = 0;
    int subBgX = 0, subBgY = 0;
};
