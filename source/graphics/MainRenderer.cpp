#include "MainRenderer.hpp"
#include "Graphic.hpp"
#include "../world/World.hpp"

MainRenderer::MainRenderer(Graphics &graphics) :
        mapId(graphics.getMainBgID()), tileMap(bgGetMapPtr(graphics.getMainBgID())) {}

void MainRenderer::setTileXY(cVec2i &pos, int tile, int palette) {
    tile |= palette << 12;
    tileMap[(pos.x % 64) + (pos.y % 64) * 64] = (uint16) tile;
}

void MainRenderer::renderTile16(Vec2i pos, int tile, int palette) {
    pos *= 2; //Convert tiles of 16 to tiles of 8
    tile *= 2 * 2;
    if (palette < 0) palette = 1;
    //Draw the 4 (8x8) tiles
    setTileXY({pos.x, pos.y}, tile, palette);
    setTileXY({pos.x + 1, pos.y}, tile + 2, palette);
    setTileXY({pos.x, pos.y + 1}, tile + 1, palette);
    setTileXY({pos.x + 1, pos.y + 1}, tile + 3, palette);
}

void MainRenderer::renderBlock(cVec2i &pos, Block block, int brightness) {
    renderTile16(pos, int(block), brightness);
}

void MainRenderer::setScroll(cVec2i &pos) {
    bgSetScroll(mapId, pos.x, pos.y);
    bgUpdate();
}

void MainRenderer::begin() {
    camRender = Vec2f(Vec2i(cam.pos * float(Graphics::blockSize))) / float(Graphics::blockSize);
    setScroll(-project({0.f, 0.f}));
}

void MainRenderer::update() {
    cam.update();
    cam.bound(0, World::sx - float(Graphics::px) / Graphics::blockSize,
              0, World::sy - float(Graphics::py) / Graphics::blockSize);
    oamUpdate(&oamMain);
    Graphic::resetSprites(true);
}

void MainRenderer::updateCenter(const Vec2f &pos) {
    cam.target = pos - Vec2f(Graphics::px, Graphics::py) / (2.f * Graphics::blockSize);
}

void MainRenderer::setCenter(const Vec2f &pos) {
    cam.setTo(pos  - Vec2f(Graphics::px, Graphics::py) / (2.f * Graphics::blockSize));
}

const Vec2f &MainRenderer::getCam() {
    return cam.pos;
}

Vec2i MainRenderer::project(cVec2f &pos) {
    return Vec2i((pos - camRender) * float(Graphics::blockSize));
}
