#pragma once
#include "graphics.h"
#include "../FixedPoint.h"
#include "../world.h"
class Particle
{
private:
	FixedPoint x;
	FixedPoint y;
	FixedPoint vx;
	FixedPoint vy;
	FixedPoint ax;
	FixedPoint ay;
	int LiveTime;
	Graphic* g;
public:
	Particle(FixedPoint x, FixedPoint y, FixedPoint vx, FixedPoint vy, FixedPoint ax, FixedPoint ay,int LiveTime, Graphic* g) : x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay), LiveTime(LiveTime), g(g) {};
	Particle(float x, float y, float vx, float vy, float ax, float ay, int LiveTime, Graphic*g );
	Particle(int x, int y, int vx, int vy, int ax, int ay, int LiveTime, Graphic* g);
	bool UpdatePhys();
	void Draw(WorldObject& world);
};

void addParticle(Particle p);

void updateParticles(WorldObject& world);