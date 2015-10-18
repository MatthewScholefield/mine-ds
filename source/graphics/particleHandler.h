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
	bool KillGraphic;
	Graphic g;
public:

	Particle(FixedPoint x, FixedPoint y, FixedPoint vx, FixedPoint vy
			, FixedPoint ax, FixedPoint ay, int LiveTime, Graphic g, bool KillGraphic = false)
	: x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay), LiveTime(LiveTime), KillGraphic(KillGraphic), g(g) { };

	Particle(float x, float y, float vx, float vy, float ax, float ay, int LiveTime, Graphic g, bool KillGraphic = false);
	Particle(int x, int y, int vx, int vy, int ax, int ay, int LiveTime, Graphic g, bool KillGraphic = false);
	~Particle();
	bool UpdatePhys();
	void Draw(WorldObject& world);
};

Graphic* getCloneWaterGraphic();


void addParticle(Particle p);

void updateParticles(WorldObject& world);
void initParticles();
void createSplash(const FixedPoint &px, const FixedPoint &py, const FixedPoint &vx, const FixedPoint &vy);