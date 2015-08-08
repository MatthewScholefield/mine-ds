#pragma once
#include <array>
#include <valarray>

using Coord = std::pair<int, int>;
using Color = std::valarray<int>;
using Rectangle = std::array<Coord, 4>;
using ColoredRect = std::pair<Color, Rectangle>;

void drawTriangle(Color color, int x1, int y1, int x2, int y2, int x3, int y3);
void drawRect(Color color, int x, int y, int sx, int sy);
void init3D();
void update3D();