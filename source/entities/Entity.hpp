#pragma once

#include "../SmoothCoord.hpp"
#include "../utils.hpp"
#include "../world/World.hpp"
#include "../graphics/Graphic.hpp"

class MainRenderer;
class World;
class Graphics;

class Entity {
    enum class Orientation {
        Left, Right
    };
public:
    explicit Entity(const Graphic &graphic, const Vec2f &size, const Vec2f &spriteSize);
    virtual void update(World &world, float dt);
    virtual void render(MainRenderer &renderer);
    virtual ~Entity() = default;

    const Vec2f &getPos();

protected:
    bool onGround(const World &world);
    const Vec2f size{6.f/16.f, 2.f}, spriteSize{1.f, 2.f};
    Graphic graphic;
    Vec2f pos{0, 0}, vel{0, 0};
    Orientation orientation{Orientation::Right};

private:
    void handleCollisions(World &world);
};
