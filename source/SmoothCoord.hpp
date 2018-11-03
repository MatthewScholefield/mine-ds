#pragma once

/**
 * Class for a LERPed coordinate
 */
struct SmoothCoord {
public:
    const float &x, &y;
    float &tx, &ty;

    SmoothCoord(float x, float y, float speed = 0.01f);
    void update();
    bool needsUpdate();
    void moveTo(float x, float y);
    void setTo(float x, float y);

private:
    float targetX, targetY, currentX, currentY;
    float speed;
};
