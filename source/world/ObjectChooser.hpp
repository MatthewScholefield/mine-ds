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

#include <type_traits>

#define onlyAtEnd(a) typename std::enable_if<sizeof...(a) == 0 > ::type

#include "../Block.hpp"

template<int a, int b>
class Entry {
public:
    static constexpr int NUM = a;
    static constexpr int PROB = b;
};

template<typename K, typename... Ts>
class ObjectChooser {
private:
    const int SUM;

public:
    ObjectChooser();
    const int getSum();
    constexpr K choose(int randVal);
    constexpr K choose();

    template<typename T, typename... Args>
    constexpr int adder();
    template<typename... Args, typename = onlyAtEnd(Args)>
    constexpr int adder();

    template<typename T, typename... Args>
    constexpr K find(int left, K block = (K) 0);
    template<typename... Args, typename = onlyAtEnd(Args)>
    constexpr K find(int left, K block);
};

#include "ObjectChooser.inl"