#include <vector>
#include "../general.h"
#include "graphics.h"
#include "particleHandler.h"

std::vector<Particle> particles;

Graphic* waterGraphic;

void initParticles()
{
	waterGraphic = new Graphic;
	loadGraphic(waterGraphic,GRAPHIC_PARTICLE,1);
}

Graphic* getCloneWaterGraphic()
{
	Graphic* g = new Graphic;
	setCloneGraphic(waterGraphic,g);
	return g;
}

Particle::~Particle()
{
	if (this->KillGraphic)
	{
			unloadGraphic(this->g);
	}
}

Particle::Particle(float x, float y, float vx, float vy, float ax, float ay, int LiveTime, Graphic* g, bool KillGraphic)
{
	this->x  = FixedPoint(true,(int)(x *FixedPoint::SCALER));
	this->vx = FixedPoint(true,(int)(vx*FixedPoint::SCALER));
	this->ax = FixedPoint(true,(int)(ax*FixedPoint::SCALER));
	this->y  = FixedPoint(true,(int)(y *FixedPoint::SCALER));
	this->vy = FixedPoint(true,(int)(vy*FixedPoint::SCALER));
	this->ay = FixedPoint(true,(int)(ay*FixedPoint::SCALER));
	this->LiveTime = LiveTime;
	this->g = g;
	this->KillGraphic = KillGraphic;
}

Particle::Particle(int x, int y, int vx, int vy, int ax, int ay, int LiveTime,Graphic * g, bool KillGraphic)
{
	Particle(FixedPoint(x),FixedPoint(y),FixedPoint(vx),FixedPoint(vy),FixedPoint(ax),FixedPoint(ay),LiveTime,g,KillGraphic);
}

bool Particle::UpdatePhys()
{
	x += vx;
	y += vy;
	vx += ax;
	vy += ay;
	return (bool)--LiveTime;
}

void Particle::Draw(WorldObject& world)
{
	showGraphic(g,x - world.camX,y - world.camY);
}

void addParticle(Particle p)
{
	particles.push_back(p);
}

void updateParticles(WorldObject& world)
{
	std::vector<Particle>::iterator it;
	for (it = particles.begin(); it != particles.end(); ++it)
	{
		bool ShouldDelete = (*it).UpdatePhys();
		(*it).Draw(world);
		if (!ShouldDelete)
		{
			--it;
			particles.erase(it+1);
		}
	}
}