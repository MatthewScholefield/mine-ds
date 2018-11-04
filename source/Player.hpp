#pragma once

#include "SmoothCoord.hpp"
#include "utils.hpp"
#include "World.hpp"

class MainRenderer;
class World;

class Player {
public:
    void update(World &world, float dt);
    void render(MainRenderer &renderer);

    const Vec2f &getPos();

private:
    Vec2f getMoveVel();

    static constexpr float speed = World::blockSize * 10;
    Vec2f pos{0, 0};
    Vec2f vel{0, 0};
};
