#pragma once

#include "SmoothCoord.hpp"
#include "utils.hpp"
#include "world/World.hpp"
#include "graphics/Graphic.hpp"

class MainRenderer;
class World;

class Player {
public:
    explicit Player(Graphics &graphics);
    void update(World &world, float dt);
    void render(MainRenderer &renderer);

    const Vec2f &getPos();

private:
    Vec2f getMoveVel();

    static constexpr float speed = 10;
    Vec2f pos{0, 0}, vel{0, 0};
    Graphic graphic;
};
