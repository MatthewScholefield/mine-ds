#pragma once

#include "../SmoothCoord.hpp"
#include "../utils.hpp"
#include "../world/World.hpp"
#include "../graphics/Graphic.hpp"
#include "Entity.hpp"

class MainRenderer;
class World;

class Player : public Entity {
public:
    explicit Player(Graphics &graphics, const Vec2f &cam);
    void update(World &world, float dt) override;

private:

    static constexpr float speed = 4.5;
    const Vec2f &cam;
    Block heldBlock = Block::PlanksWood;
};
