#pragma once

#include "SmoothCoord.hpp"
#include "utils.hpp"
#include "world/World.hpp"
#include "graphics/Graphic.hpp"

class MainRenderer;
class World;

class Player {
public:
    explicit Player(Graphics &graphics, const Vec2f &cam);
    void update(World &world, float dt);
    void handleCollisions(World &world);
    void render(MainRenderer &renderer);

    const Vec2f &getPos();

private:
    bool onGround(const World &world);

    static constexpr float speed = 4.5;
    const Vec2f size{6.f/16.f, 2.f}, spriteSize{1.f, 2.f};
    Vec2f pos{0, 0}, vel{0, 0};
    const Vec2f &cam;
    Graphic graphic;
    Block heldBlock = Block::PlanksWood;
};
