/*
 * Copyright (C) 2016 Matthew D. Scholefield
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "State.hpp"

State *State::air = new AirState(),
        *State::grass = new GrassState(),
        *State::snowGrass = new SnowGrassState(),
        *State::dirt = new DirtState(),
        *State::stone = new StoneState();

void State::enter() {}

State *State::update(float noise, float threshold, Biome biome) {
    if (noise < threshold)
        return air;
    else
        return updateVirtual(noise, biome);
}

State *AirState::updateVirtual(float noise, Biome biome) {
    return biome == Biome::SNOW ? State::snowGrass : State::grass;
}

Block AirState::getBlock() {
    return Block::Air;
}

State *GrassState::updateVirtual(float noise, Biome biome) {
    return State::dirt;
}

Block GrassState::getBlock() {
    return Block::Grass;
}

State *SnowGrassState::updateVirtual(float noise, Biome biome) {
    return State::dirt;
}

Block SnowGrassState::getBlock() {
    return Block::GrassSnow;
}

void DirtState::enter() {
    count = 0;
}

State *DirtState::updateVirtual(float noise, Biome biome) {
    if (++count >= NUM_DIRT)
        return State::stone;
    else
        return State::dirt;
}

Block DirtState::getBlock() {
    return Block::Dirt;
}

State *StoneState::updateVirtual(float noise, Biome biome) {
    return State::stone;
}

Block StoneState::getBlock() {
    return STONES.choose();
}