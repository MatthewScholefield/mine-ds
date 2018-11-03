#include <nds.h>
#include <font.h>
#include "Graphic.hpp"
#include "../utils.hpp"
#include "SubRenderer.hpp"
#include "Graphics.hpp"
#include "Font.hpp"


SubRenderer::SubRenderer(Graphics &graphics) :
        graphics(graphics), mapId(graphics.getSubBgID()), tileMap(bgGetMapPtr(graphics.getSubBgID())),
        consoleId(graphics.getFont().getConsoleId()) {
    drawBackground();
    set(0, 0);
}

void SubRenderer::setTileXY(int x, int y, int tile, int palette, int flip) {
    auto tileData = (uint16) tile;
    tileData |= palette << 12;
    tileData |= flip << 10;
    tileMap[x + y * 64] = tileData;
}

void SubRenderer::setTile(int x, int y, int tile) {
    setTileXY(x, y, tile, 0, 0);
}

void SubRenderer::setTile(int x, int y, int tile, int flip) {
    setTileXY(x, y, tile, 0, flip);
}

void SubRenderer::drawBackOffset(int offX, int offY, bool mineDS) {
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 32; ++j) {
            setTile(j + offX, i + offY, ((i % 2) ? 90 : 122) + j % 2);
        }
    }
    if (mineDS) {
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 7; ++j) {
                setTile(i + offX + 3, j + offY, i + (j * 32)); //Draw the MineDS Logo!
            }
        }
    }
}

void SubRenderer::drawBackground(bool firstSlot, bool mineDS) //Draws dirt background and MineDS Logo
{
    drawBackOffset(firstSlot ? 0 : 32, 0, mineDS);
}

void SubRenderer::move(int dx, int dy) {
    subBgX += dx;
    subBgY += dy;
    if (subBgY + 192 > 512) {
        subBgY = 512 - 192;
    } else if (subBgY < 0)
        subBgY = 0;
}

void SubRenderer::set(int x, int y) {
    bgSetScroll(consoleId, (int) (subBgCalcX = subBgX = x), (int) (subBgCalcY = subBgY = y));
    bgSetScroll(graphics.getSubBgID(), subBgX, subBgY);
    bgUpdate();
}

int SubRenderer::getScrollX() {
    return roundInt(subBgCalcX) % 512;
}

int SubRenderer::getScrollY() {
    return roundInt(subBgCalcY);
}

void SubRenderer::update() {
    if (roundInt(subBgCalcX) != subBgX || roundInt(subBgCalcY) != subBgY) {
        subBgCalcX += (double(subBgX) - subBgCalcX) * 0.08;
        subBgCalcY += (double(subBgY) - subBgCalcY) * 0.08;
        bgSetScroll(graphics.getSubBgID(), roundInt(subBgCalcX), roundInt(subBgCalcY));
        bgSetScroll(consoleId, roundInt(subBgCalcX), roundInt(subBgCalcY));
        bgUpdate();
    }
    oamUpdate(&oamSub);
    Graphic::resetSprites(false);
}

void SubRenderer::drawButton(int x, int y, int sizex) {
    sizex -= 1;
    setTile(x, y + 1, 27);
    setTile(x, y, 26);
    setTile(x, y + 2, 26, V_FLIP);
    setTile(x + sizex, y + 1, 27, H_FLIP);
    setTile(x + sizex, y, 26, H_FLIP);
    setTile(x + sizex, y + 2, 26, BOTH_FLIP);

    int i;
    for (i = 0; i < sizex - 1; ++i) {
        setTile(x + 1 + i, y, 30);
    }
    for (i = 0; i < sizex - 1; ++i) {
        setTile(x + 1 + i, y + 2, 30, V_FLIP);
    }
    for (i = 0; i < sizex - 1; ++i) {
        setTile(x + 1 + i, y + 1, 28 + (i % 2));
    }
}

void SubRenderer::drawButtonColored(int x, int y, int sizex) {
    sizex -= 1;
    setTile(x, y, 58);
    setTile(x, y + 2, 58, V_FLIP);
    setTile(x + sizex, y, 58, H_FLIP);
    setTile(x + sizex, y + 2, 58, BOTH_FLIP);
    setTile(x, y + 1, 59);
    setTile(x + sizex, y + 1, 59, H_FLIP);

    int i;
    for (i = 0; i < sizex - 1; ++i) {
        setTile(x + 1 + i, y, 62);
    }
    for (i = 0; i < sizex - 1; ++i) {
        setTile(x + 1 + i, y + 2, 62, V_FLIP);
    }
    for (i = 0; i < sizex - 1; ++i) {
        setTile(x + 1 + i, y + 1, 60 + (i % 2));
    }
}

void SubRenderer::drawBoxCenter(int x, int y, int lx, int ly) //Draws a box without borders
{
    for (int i = 0; i < lx; ++i) {
        for (int j = 0; j < ly; ++j) {
            setTile(x + i, y + j, 28 + (i + 1) % 2);
        }
    }
}

void SubRenderer::drawBoxFrame(int x, int y, int lx, int ly) {
    setTile(x, y, 26); //Top-Left Corner
    setTile(x, y + ly - 1, 26, V_FLIP); //Bottom-Left Corner
    setTile(x + lx - 1, y + ly - 1, 26, BOTH_FLIP); //Bottom-Right Corner
    setTile(x + lx - 1, y + 0, 26, H_FLIP); //Top-Right Corner
    for (int i = 1; i < ly - 1; ++i) {
        setTile(x + lx - 1, y + i, 27, H_FLIP); //Right Edge
        setTile(x, y + i, 27); //Left Edge
    }
    for (int i = 1; i < lx - 1; ++i) {
        setTile(x + i, y, 30); //Top Edge
        setTile(x + i, y + ly - 1, 30, V_FLIP); //Bottom Edge
    }
}

void SubRenderer::drawBox(int x, int y, int lx, int ly) {
    drawBoxFrame(x, y, lx, ly);
    drawBoxCenter(x + 1, y + 1, lx - 2, ly - 2);
}
