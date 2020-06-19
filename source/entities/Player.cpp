#include "Player.hpp"
#include "../graphics/MainRenderer.hpp"
#include "../graphics/Graphics.hpp"
#include "../Game.hpp"


Player::Player(Graphics &graphics, const Vec2f &cam) :
        Entity(Graphic(graphics, GraphicType::MOB_ANIM, 0, true, 14), {4.f / 16.f, 2.f}, {1.f, 2.f}), cam(cam) {}

void Player::respawn(const Game &g, MainRenderer &mainRenderer) {
    int spawnX = rand() % World::sx, spawnY = 0;
    for (; spawnY < World::sy && isWalkThrough(g.world.fg[spawnX][spawnY]); ++spawnY);
    pos.x = spawnX + 0.5f;
    pos.y = spawnY - 1;
    mainRenderer.setCenter({pos.x, pos.y - Graphics::py / (2.f * Graphics::blockSize)});
}

void Player::update(Game &g, float dt) {
    if (std::abs(vel.x) > 0.5) {
        graphic.animate();
    } else {
        graphic.setFrame(0);
    }
    vel.x *= 0.9f;
    bool isOnGround = onGround(g.world);
    if (keysHeld() & KEY_RIGHT) {
        if (isOnGround) {
            vel.x = speed;
        } else {
            vel.x += 25.f * dt;
        }
        timeTillStep -= dt;
    } else if (keysHeld() & KEY_LEFT) {
        if (isOnGround) {
            vel.x = -speed;
        } else {
            vel.x -= 25.f * dt;
        }
        timeTillStep -= dt;
    } else {
        if (isOnGround) {
            vel.x = 0;
        }
        timeTillStep = 0.f;
    }
    
    if (timeTillStep < 0.f) {
        g.soundSystem.playSound(SoundAudio::Grass, SoundType::Step, 255, 128);
        timeTillStep = stepAudioInterval;
    }
    if (isOnGround) {
        if (keysHeld() & KEY_UP) {
            vel.y = -jumpSpeed;
        } else {
            jumpState = jumpAny;
        }
    } else if (keysHeld() & KEY_UP && tryWallJump(g.world)) {
        vel.y = -speed;
    }

    if (keysDown() & KEY_TOUCH) {
        touchPosition pos;
        touchRead(&pos);
        auto touchBlock = Vec2i(cam + Vec2f(pos.px - 1, pos.py - 1) / float(Graphics::blockSize));
        Block &block = g.world.fg[touchBlock.x][touchBlock.y];
        if (block != Block::Air) {
            block = Block::Air;
        } else {
            block = heldBlock;
        }
        g.world.calculateBrightness(touchBlock.x, touchBlock.y);
    }

    Entity::update(g, dt);
}

bool Player::tryWallJump(const World &world) {
    if (vel.y < -0.1f) {
        return false;
    }
    bool wallRight = !isWalkThrough(world.fg[int(pos.x + size.x / 2.f + Graphics::eps)][int(pos.y + size.y / 2.f + Graphics::eps)]);
    bool wallLeft = !isWalkThrough(world.fg[int(pos.x - size.x / 2.f - Graphics::eps)][int(pos.y + size.y / 2.f + Graphics::eps)]);
    if (wallLeft && jumpState & jumpLeft) {
        jumpState = jumpRight;
        vel.x = 2.01f * speed;
    } else if (wallRight && jumpState & jumpRight) {
        jumpState = jumpLeft;
        vel.x = -2.0f * speed;
    } else {
        return false;
    }
    return true;
}
