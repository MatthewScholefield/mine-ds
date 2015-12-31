#pragma once
#include "graphics.h"
#include "../FixedPoint.h"
#include "../world/WorldObject.h"

class Particle
{
private:
	FixedPoint x;
	FixedPoint y;
	FixedPoint vx;
	FixedPoint vy;
	FixedPoint ax;
	FixedPoint ay;
	int liveTime;
	bool killGraphic;
	Graphic g;
public:

	Particle(FixedPoint x, FixedPoint y, FixedPoint vx, FixedPoint vy
			, FixedPoint ax, FixedPoint ay, int LiveTime, Graphic g, bool killGraphic = false)
	: x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay), liveTime(LiveTime), killGraphic(killGraphic), g(g) { };

	Particle(float x, float y, float vx, float vy, float ax, float ay, int liveTime, Graphic g, bool killGraphic = false);
	Particle(int x, int y, int vx, int vy, int ax, int ay, int liveTime, Graphic g, bool killGraphic = false);

	~Particle() { }
	bool updatePhys();
	void draw(World& world);
};

Graphic* getCloneWaterGraphic();


void addParticle(Particle p);

void updateParticles(World& world);
void createSplash(const FixedPoint &px, const FixedPoint &py, const FixedPoint &vx, const FixedPoint &vy);