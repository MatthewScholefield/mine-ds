#pragma once

#include "../SmoothCoord.hpp"
#include "../utils.hpp"
#include "../world/World.hpp"
#include "../graphics/Graphic.hpp"
#include "Entity.hpp"

class MainRenderer;
class Game;

class Player : public Entity {
public:
    explicit Player(Graphics &graphics, const Vec2f &cam);
    void respawn(const Game &game, MainRenderer &mainRenderer);
    void update(Game &game, float dt) override;

private:

    static constexpr float speed = 4.5f, stepAudioInterval = 0.2f;
    float timeTillStep = 0.f;
    const Vec2f &cam;
    Block heldBlock = Block::PlanksWood;
};
