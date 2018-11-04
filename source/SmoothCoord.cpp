#include "SmoothCoord.hpp"
#include "utils.hpp"

SmoothCoord::SmoothCoord(const Vec2f &pos, float speed) :
        writeablePos(pos),
        speed(speed), x(writeablePos.x), y(writeablePos.y), pos(writeablePos), target(pos) {}

void SmoothCoord::update() {
    writeablePos += (target - pos) * speed;
}

bool SmoothCoord::needsUpdate() {
    return roundInt(pos.x) != int(target.x) || roundInt(pos.y) != int(target.y);
}

void SmoothCoord::moveTo(const Vec2f &pos) {
    target = pos;
}

void SmoothCoord::setTo(const Vec2f &pos) {
    target = writeablePos = pos;
}
