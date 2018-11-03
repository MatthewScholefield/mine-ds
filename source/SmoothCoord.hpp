#pragma once

/**
 * Class for a LERPed coordinate
 */
struct SmoothCoord {
public:
    const float &x, &y;
    float &tx, &ty;

    SmoothCoord(float x, float y, float speed = 0.01f) :
            x(currentX), y(currentY), tx(targetX), ty(targetY), targetX(x), targetY(y), currentX(x), currentY(y), speed(speed) {}

    void update() {
        currentX += (targetX - currentX) * speed;
        currentY += (targetY - currentY) * speed;
    }

    void moveTo(float x, float y) {
        targetX = x;
        targetY = y;
    }

    void setTo(float x, float y) {
        targetX = currentX = x;
        targetY = currentY = y;
    }

private:
    float targetX, targetY, currentX, currentY;
    float speed;
};
