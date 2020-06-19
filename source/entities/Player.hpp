#pragma once

#include "../SmoothCoord.hpp"
#include "../utils.hpp"
#include "../world/World.hpp"
#include "../graphics/Graphic.hpp"
#include "Entity.hpp"

class MainRenderer;
class Game;

class Player : public Entity {
    enum WallJumpState {
        jumpAny = 0b11,
        jumpLeft = 0b10,
        jumpRight = 0b01
    };
public:
    explicit Player(Graphics &graphics, const Vec2f &cam);
    void respawn(const Game &game, MainRenderer &mainRenderer);
    void update(Game &game, float dt) override;

private:
    bool tryWallJump(const World &world);

    static constexpr float speed = 4.5f, jumpSpeed = 4.55f, stepAudioInterval = 0.2f;
    float timeTillStep = 0.f;
    const Vec2f &cam;
    Block heldBlock = Block::PlanksWood;
    WallJumpState jumpState = jumpAny;
};
