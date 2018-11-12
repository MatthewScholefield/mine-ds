#include "Block.hpp"

bool isWalkThrough(Block block) {
    switch (block) {
        case Block::Air:
        case Block::FlowerYellow:
        case Block::FlowerRed:
        case Block::SnowTop:
        case Block::Torch:
        case Block::Ladder:
        case Block::Shrub:
        case Block::GrassTall:
        case Block::MushroomBrown:
        case Block::MushroomRed:
        case Block::SaplingJungle:
        case Block::SaplingOak:
        case Block::SaplingSpruce:
        case Block::DoorOpenTop:
        case Block::DoorOpenBottom:
        case Block::Water:
            return true;

        default:
            return false;
    }
}
