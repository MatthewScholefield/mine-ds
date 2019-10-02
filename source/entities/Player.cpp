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
    if (vel.x != 0) {
        graphic.animate();
    } else {
        graphic.setFrame(0);
    }
    if (keysHeld() & KEY_RIGHT) {
        vel.x = speed;
        timeTillStep -= dt;
    } else if (keysHeld() & KEY_LEFT) {
        vel.x = -speed;
        timeTillStep -= dt;
    } else {
        vel.x = 0;
        timeTillStep = 0.f;
    }
    if (timeTillStep < 0.f) {
        g.soundSystem.playSound(SoundAudio::Grass, SoundType::Step, 255, 128);
        timeTillStep = stepAudioInterval;
    }
    if (keysHeld() & KEY_UP && onGround(g.world)) {
        vel.y = -speed;
    }

    if (keysDown() & KEY_TOUCH) {
        touchPosition pos;
        touchRead(&pos);
        auto touchBlock = Vec2i(cam + Vec2f(pos.px, pos.py) / float(Graphics::blockSize));
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
