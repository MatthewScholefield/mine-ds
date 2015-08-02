#pragma once
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
void init3D();
void update3D();

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
};