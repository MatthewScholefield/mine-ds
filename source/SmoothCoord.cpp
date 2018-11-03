#include "SmoothCoord.hpp"
#include "utils.hpp"

SmoothCoord::SmoothCoord(float x, float y, float speed) :
        x(currentX), y(currentY), tx(targetX), ty(targetY), targetX(x), targetY(y), currentX(x), currentY(y),
        speed(speed) {}

void SmoothCoord::update() {
    currentX += (targetX - currentX) * speed;
    currentY += (targetY - currentY) * speed;
}

bool SmoothCoord::needsUpdate() {
    return roundInt(x) != int(tx) || roundInt(y) != int(ty);
}

void SmoothCoord::moveTo(float x, float y) {
    targetX = x;
    targetY = y;
}

void SmoothCoord::setTo(float x, float y) {
    targetX = currentX = x;
    targetY = currentY = y;
}
