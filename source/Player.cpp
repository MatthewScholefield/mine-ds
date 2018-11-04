#include "Player.hpp"
#include "graphics/MainRenderer.hpp"
#include "World.hpp"


void Player::update(World &world, float dt) {
    vel = getMoveVel();
    pos += vel * dt;
}

void Player::render(MainRenderer &renderer) {
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
