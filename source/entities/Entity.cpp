#include "Entity.hpp"

#include "../graphics/MainRenderer.hpp"
#include "../Game.hpp"


Entity::Entity(const Graphic &graphic, const Vec2f &size, const Vec2f &spriteSize) :
        size(size), spriteSize(spriteSize), graphic(graphic) {}

void Entity::update(Game &g, float dt) {
    vel += g.world.gravity * dt;
    if (vel.x > 0) {
        orientation = Orientation::Right;
    } else if (vel.x < 0) {
        orientation = Orientation::Left;
    }
    pos += vel * dt;
    handleCollisions(g.world);
    makeBound(pos,
              size.x / 2, float(World::sx) - size.x / 2,
              size.y / 2, float(World::sy) - size.y / 2);
}

void Entity::render(MainRenderer &renderer) {
    graphic.draw(renderer.project(pos - spriteSize / 2.f), orientation == Orientation::Left);
}

const Vec2f &Entity::getPos() {
    return pos;
}

void Entity::handleCollisions(World &world) {
    auto at = [&](float x, float y) {
        return world.fg[int(x)][int(y)];
    };

    float left = pos.x - size.x / 2;
    float top = pos.y - size.y / 2;
    float right = pos.x + size.x / 2;
    float bottom = pos.y + size.y / 2;
    float center = pos.y;

    bool stuckBottomLeft = !isWalkThrough(at(left, bottom));
    bool stuckBottomRight = !isWalkThrough(at(right, bottom));
    bool stuckCenterLeft = !isWalkThrough(at(left, center));
    bool stuckCenterRight = !isWalkThrough(at(right, center));
    bool stuckTopLeft = !isWalkThrough(at(left, top));
    bool stuckTopRight = !isWalkThrough(at(right, top));

    float inRight = right - (int) right;
    float inLeft = 1.f - (left - (int) left);

    float inBottom = bottom - (int) bottom;
    float inTop = 1.f - (top - (int) top);

    if (stuckCenterLeft) {
        pos.x += inLeft;
        vel.x = 0;
        inLeft = 0;
    } else if (stuckCenterRight) {
        pos.x -= inRight;
        vel.x = 0;
        inRight = 0;
    }

    if (stuckBottomLeft && stuckBottomRight) {
        pos.y -= inBottom;
        vel.y = 0;
    } else if (stuckBottomLeft) {
        if (inBottom < inLeft && !stuckTopLeft) {
            pos.y -= inBottom;
            vel.y = 0;
        } else {
            pos.x += inLeft;
            vel.x = 0;
        }
    } else if (stuckBottomRight) {
        if (inBottom < inRight && !stuckTopRight) {
            pos.y -= inBottom;
            vel.y = 0;
        } else {
            pos.x -= inRight;
            vel.x = 0;
        }
    }

    if (stuckTopLeft && stuckTopRight) {
        pos.y += inTop;
        vel.y = 0;
    } else if (stuckTopLeft) {
        if (inTop < inLeft && !stuckBottomLeft) {
            pos.y += inTop;
            vel.y = 0;
        } else {
            pos.x += inLeft;
            vel.x = 0;
        }
    } else if (stuckTopRight) {
        if (inTop < inRight && !stuckBottomRight) {
            pos.y += inTop;
            vel.y = 0;
        } else {
            pos.x -= inRight;
            vel.x = 0;
        }
    }
}

bool Entity::onGround(const World &world) {
    return !isWalkThrough(world.fg[int(pos.x)][int(pos.y + 1.01)]);
}

