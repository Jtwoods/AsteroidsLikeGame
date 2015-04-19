#pragma once
#ifndef SHOTEXPLOSION_H
#define SHOTEXPLOSION_H
#include "Particle.h"
#include "GlobalAsteroidVariables.h"
#include "Explosion.h"
#include <list>
#include <memory>

using namespace std;

class ShotExplosion: public Explosion
{

public:
	ShotExplosion(float x, float y, float xVel, float yVel, float red, float green, float blue);
	void draw();
private:
	float red;
	float green;
	float blue;
	list<shared_ptr<GameObject>> particles;
};

#endif