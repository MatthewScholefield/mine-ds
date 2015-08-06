#pragma once

template <class T>
class Pair3
{
public:
	T elements[3];

	Pair3(T a, T b, T c)
	{
		elements[0] = a;
		elements[1] = b;
		elements[2] = c;
	}

	Pair3<T> operator/(T t)
	{
		return Pair3(elements[0] / t, elements[1] / t, elements[2] / t);
	}
};

void drawTriangle(Pair3<int> color, int x1, int y1, int x2, int y2, int x3, int y3);
void drawRect(Pair3<int> color, int x, int y, int sx, int sy);
void init3D();
void update3D();