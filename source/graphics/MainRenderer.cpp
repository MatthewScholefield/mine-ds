#include "MainRenderer.hpp"
#include "Graphic.hpp"
#include "../world/World.hpp"

MainRenderer::MainRenderer(Graphics &graphics) :
        mapId(graphics.getMainBgID()), tileMap(bgGetMapPtr(graphics.getMainBgID())) {}

void MainRenderer::setTileXY(int x, int y, int tile, int palette) {
    tile |= palette << 12;
    tileMap[(x % 64) + (y % 64) * 64] = (uint16) tile;
}

void MainRenderer::renderTile16(int x, int y, int tile, int palette) {
    x *= 2; //Convert tiles of 16 to tiles of 8
    y *= 2;
    tile *= 2 * 2;
    if (palette < 0) palette = 1;
    //Draw the 4 (8x8) tiles
    setTileXY(x, y, tile, palette);
    setTileXY(x + 1, y, tile + 2, palette);
    setTileXY(x, y + 1, tile + 1, palette);
    setTileXY(x + 1, y + 1, tile + 3, palette);
}

void MainRenderer::renderBlock(int i, int j, Block block) {
    int brightness = 15;
    renderTile16(i, j, int(block), brightness);
}

void MainRenderer::setScroll(int x, int y) {
    bgSetScroll(mapId, x, y);
    bgUpdate();
}

void MainRenderer::update() {
    cam.update();
    oamUpdate(&oamMain);
    Graphic::resetSprites(true);
}

void MainRenderer::updateCenter(const Vec2f &pos) {
    cam.target = pos - Vec2f(Graphics::px, Graphics::py) / (2.f * World::blockSize);
}

const Vec2f &MainRenderer::getCam() {
    return cam.pos;
}
