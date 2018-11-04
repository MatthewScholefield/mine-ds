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

#pragma once

#include "ObjectChooser.hpp"
#include "Biome.hpp"
#include "../Block.hpp"

class State {
public:
    virtual void enter();
    virtual State *update(float noise, float threshold, Biome biome);
    virtual Block getBlock() = 0;

    static State *air, *grass, *snowGrass, *dirt, *stone;
    virtual ~State() = default;
protected:
    virtual State *updateVirtual(float noise, Biome biome) = 0;
};

class AirState : public State {
public:
    State *updateVirtual(float noise, Biome biome) override;
    Block getBlock() override;
};

class GrassState : public State {
public:
    State *updateVirtual(float noise, Biome biome) override;
    Block getBlock() override;
};

class SnowGrassState : public State {
public:
    State *updateVirtual(float noise, Biome biome) override;
    Block getBlock() override;
};

class DirtState : public State {
public:
    void enter() override;
    State *updateVirtual(float noise, Biome biome) override;
    Block getBlock() override;
private:
    int count;
    const int NUM_DIRT = 4;
};

class StoneState : public State {
public:
    State *updateVirtual(float noise, Biome biome) override;
    Block getBlock() override;
private:
    ObjectChooser<Block,
            Entry<int(Block::Stone), 90>,
            Entry<int(Block::OreCoal), 10>,
            Entry<int(Block::OreIron), 5>,
            Entry<int(Block::OreGold), 3>,
            Entry<int(Block::OreRedstone), 4>,
            Entry<int(Block::OreDiamond), 1>
    > STONES;
};