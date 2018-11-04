#pragma once

#include "utils.hpp"

/**
 * Class for a LERPed coordinate
 */
struct SmoothCoord {
    Vec2f writeablePos;
    float speed;
public:
    const float &x, &y;
    const Vec2f &pos;
    Vec2f target;

    explicit SmoothCoord(const Vec2f &pos, float speed = 0.01f);
    void update();
    bool needsUpdate();
    void moveTo(const Vec2f &pos);
    void setTo(const Vec2f &pos);
};
