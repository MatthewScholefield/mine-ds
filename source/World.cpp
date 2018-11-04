#include "World.hpp"
#include "graphics/MainRenderer.hpp"
#include "utils.hpp"

void World::update() {
}

void World::generate() {
    for (auto &row : blocks){
        for (auto &block : row) {
            if (rand() % 4 == 0) {
                block = Block(rand() % int(Block::Length));
            }
        }
    }
}

void World::render(MainRenderer &renderer) {
    auto &cam = renderer.getCam();
    renderer.setScroll(int(cam.x), int(cam.y));
    const int blockX = int(cam.x) / blockSize;
    const int blockY = int(cam.y) / blockSize;
    for (int renderX = blockX; renderX <= blockX + Graphics::px / blockSize; ++renderX) {
        for (int renderY = blockY; renderY <= blockY + Graphics::py / blockSize; ++renderY) {
            renderer.renderBlock(renderX, renderY, blocks[renderX][renderY]);
        }
    }
}
