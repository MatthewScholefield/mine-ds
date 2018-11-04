#include "Player.hpp"
#include "graphics/MainRenderer.hpp"
#include "World.hpp"


Player::Player(Graphics &graphics) : graphic(graphics, GraphicType::MOB_ANIM, 0, true, 14) {}

void Player::update(World &world, float dt) {
    vel = getMoveVel();
    if (vel.x != 0 || vel.y != 0) {
        graphic.animate();
    } else {
        graphic.setFrame(0);
    }
    pos += vel * dt;
}

void Player::render(MainRenderer &renderer) {
    auto p = Vec2i(pos * float(World::blockSize) - renderer.getCam());
    graphic.draw(p.x, p.y);
}

const Vec2f &Player::getPos() {
    return pos;
}

Vec2f Player::getMoveVel() {
    Vec2f vel{0, 0};
    if (keysHeld() & KEY_RIGHT) {
        vel.x = speed;
    } else if (keysHeld() & KEY_LEFT) {
        vel.x = -speed;
    }
    if (keysHeld() & KEY_DOWN) {
        vel.y = speed;
    } else if (keysHeld() & KEY_UP) {
        vel.y = -speed;
    }
    return vel;
}
