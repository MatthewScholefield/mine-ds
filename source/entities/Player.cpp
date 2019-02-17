#include "Player.hpp"
#include "../graphics/MainRenderer.hpp"
#include "../graphics/Graphics.hpp"


Player::Player(Graphics &graphics, const Vec2f &cam) :
        Entity(Graphic(graphics, GraphicType::MOB_ANIM, 0, true, 14), {4.f / 16.f, 2.f}, {1.f, 2.f}), cam(cam) {}

void Player::respawn(const World &world, MainRenderer &mainRenderer) {
    int spawnX = rand() % World::sx, spawnY = 0;
    for (; spawnY < World::sy && isWalkThrough(world.fg[spawnX][spawnY]); ++spawnY);
    pos.x = spawnX + 0.5f;
    pos.y = spawnY - 1;
    mainRenderer.setCenter({pos.x, pos.y - Graphics::py / (2.f * Graphics::blockSize)});
}

void Player::update(World &world, float dt) {
    if (vel.x != 0) {
        graphic.animate();
    } else {
        graphic.setFrame(0);
    }
    if (keysHeld() & KEY_RIGHT) {
        vel.x = speed;
    } else if (keysHeld() & KEY_LEFT) {
        vel.x = -speed;
    } else {
        vel.x = 0;
    }
    if (keysHeld() & KEY_UP && onGround(world)) {
        vel.y = -speed;
    }

    if (keysDown() & KEY_TOUCH) {
        touchPosition pos;
        touchRead(&pos);
        auto touchBlock = Vec2i(cam + Vec2f(pos.px, pos.py) / float(Graphics::blockSize));
        Block &block = world.fg[touchBlock.x][touchBlock.y];
        if (block != Block::Air) {
            block = Block::Air;
        } else {
            block = heldBlock;
        }
        world.calculateBrightness(touchBlock.x, touchBlock.y);
    }

    Entity::update(world, dt);
}
