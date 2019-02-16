#include "Player.hpp"
#include "../graphics/MainRenderer.hpp"


Player::Player(Graphics &graphics, const Vec2f &cam) :
        Entity(Graphic(graphics, GraphicType::MOB_ANIM, 0, true, 14), {6.f / 16.f, 2.f}, {1.f, 2.f}), cam(cam) {}

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
        auto touchBlock = Vec2i(cam + Vec2f(pos.px, pos.py) / float(World::blockSize));
        Block &block = world[touchBlock];
        if (block != Block::Air) {
            block = Block::Air;
        } else {
            block = heldBlock;
        }
    }

    Entity::update(world, dt);
}
