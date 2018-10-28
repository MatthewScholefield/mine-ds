#include <nds.h>
#include <font.h>
#include "Graphic.hpp"
#include "utils.hpp"
#include "subBgHandler.hpp"
#include "TitleGraphicsSystem.hpp"
#include "GraphicsSystem.hpp"
#include "TitleFontSystem.hpp"


TitleGraphicsSystem::TitleGraphicsSystem(TitleFontSystem &fontSystem, GraphicsSystem &graphics)
        : fontSystem(fontSystem), graphics(graphics), bgptr(graphics.getSubBgPtr()) {
    drawBackground();
}

void TitleGraphicsSystem::setSubTileXY(int x, int y, int tile, int palette, int flip) {
    auto tileData = (uint16) tile;
    tileData |= palette << 12;
    tileData |= flip << 10;
    bgptr[x + y * 64] = tileData;
}

void TitleGraphicsSystem::setSubBgTile(int x, int y, int tile) {
    setSubTileXY(x, y, tile, 0, 0);
}

void TitleGraphicsSystem::setSubBgTile(int x, int y, int tile, int flip) {
    setSubTileXY(x, y, tile, 0, flip);
}

void TitleGraphicsSystem::drawBackOffset(int offX, int offY, bool mineDS) {
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 32; ++j) {
            setSubBgTile(j + offX, i + offY, ((i % 2) ? 90 : 122) + j % 2);
        }
    }
    if (mineDS) {
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 7; ++j) {
                setSubBgTile(i + offX + 3, j + offY, i + (j * 32)); //Draw the MineDS Logo!
            }
        }
    }
}

void TitleGraphicsSystem::drawBackground(bool firstSlot, bool mineDS) //Draws dirt background and MineDS Logo
{
    drawBackOffset(firstSlot ? 0 : 32, 0, mineDS);
}

int TitleGraphicsSystem::getSubBgID() {
    return subBgID;
}

void TitleGraphicsSystem::moveSubBg(int dX, int dY) {
    subBgX += dX;
    subBgY += dY;
    if (subBgY + 192 > 512)
        subBgY = 512 - 192;
    else if (subBgY < 0)
        subBgY = 0;
}

void TitleGraphicsSystem::setSubBg(int x, int y) {
    bgSetScroll(fontSystem.getConsoleId(), (int) (subBgCalcX = subBgX = x), (int) (subBgCalcY = subBgY = y));
    bgSetScroll(graphics.getSubBgID(), subBgX, subBgY);
    bgUpdate();
}

int TitleGraphicsSystem::getScrollX() {
    return roundInt(subBgCalcX) % 512;
}

int TitleGraphicsSystem::getScrollY() {
    return roundInt(subBgCalcY);
}

void TitleGraphicsSystem::updateSubBG() {
    if (roundInt(subBgCalcX) != subBgX || roundInt(subBgCalcY) != subBgY) {
        subBgCalcX += (double(subBgX) - subBgCalcX) * 0.08;
        subBgCalcY += (double(subBgY) - subBgCalcY) * 0.08;
        bgSetScroll(graphics.getSubBgID(), roundInt(subBgCalcX), roundInt(subBgCalcY));
        bgSetScroll(fontSystem.getConsoleId(), roundInt(subBgCalcX), roundInt(subBgCalcY));
        bgUpdate();
    }
    oamUpdate(&oamSub);
    Graphic::resetSprites(false);
}

void TitleGraphicsSystem::drawButton(int x, int y, int sizex) {
    sizex -= 1;
    setSubBgTile(x, y + 1, 27);
    setSubBgTile(x, y, 26);
    setSubBgTile(x, y + 2, 26, V_FLIP);
    setSubBgTile(x + sizex, y + 1, 27, H_FLIP);
    setSubBgTile(x + sizex, y, 26, H_FLIP);
    setSubBgTile(x + sizex, y + 2, 26, BOTH_FLIP);

    int i;
    for (i = 0; i < sizex - 1; ++i)
        setSubBgTile(x + 1 + i, y, 30);
    for (i = 0; i < sizex - 1; ++i)
        setSubBgTile(x + 1 + i, y + 2, 30, V_FLIP);
    for (i = 0; i < sizex - 1; ++i)
        setSubBgTile(x + 1 + i, y + 1, 28 + (i % 2));
}

void TitleGraphicsSystem::drawButtonColored(int x, int y, int sizex) {
    sizex -= 1;
    setSubBgTile(x, y, 58);
    setSubBgTile(x, y + 2, 58, V_FLIP);
    setSubBgTile(x + sizex, y, 58, H_FLIP);
    setSubBgTile(x + sizex, y + 2, 58, BOTH_FLIP);
    setSubBgTile(x, y + 1, 59);
    setSubBgTile(x + sizex, y + 1, 59, H_FLIP);

    int i;
    for (i = 0; i < sizex - 1; ++i)
        setSubBgTile(x + 1 + i, y, 62);
    for (i = 0; i < sizex - 1; ++i)
        setSubBgTile(x + 1 + i, y + 2, 62, V_FLIP);
    for (i = 0; i < sizex - 1; ++i)
        setSubBgTile(x + 1 + i, y + 1, 60 + (i % 2));
}

void TitleGraphicsSystem::drawBoxCenter(int x, int y, int lx, int ly) //Draws a box without borders
{
    for (int i = 0; i < lx; ++i)
        for (int j = 0; j < ly; ++j)
            setSubBgTile(x + i, y + j, 28 + (i + 1) % 2);
}

void TitleGraphicsSystem::drawBoxFrame(int x, int y, int lx, int ly) {
    setSubBgTile(x, y, 26); //Top-Left Corner
    setSubBgTile(x, y + ly - 1, 26, V_FLIP); //Bottom-Left Corner
    setSubBgTile(x + lx - 1, y + ly - 1, 26, BOTH_FLIP); //Bottom-Right Corner
    setSubBgTile(x + lx - 1, y + 0, 26, H_FLIP); //Top-Right Corner
    for (int i = 1; i < ly - 1; ++i) {
        setSubBgTile(x + lx - 1, y + i, 27, H_FLIP); //Right Edge
        setSubBgTile(x, y + i, 27); //Left Edge
    }
    for (int i = 1; i < lx - 1; ++i) {
        setSubBgTile(x + i, y, 30); //Top Edge
        setSubBgTile(x + i, y + ly - 1, 30, V_FLIP); //Bottom Edge
    }
}

void TitleGraphicsSystem::drawBox(int x, int y, int lx, int ly) {
    drawBoxFrame(x, y, lx, ly);
    drawBoxCenter(x + 1, y + 1, lx - 2, ly - 2);
}

TitleFontSystem &TitleGraphicsSystem::getTitleFont() {
    return fontSystem;
}
