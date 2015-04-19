#pragma once
#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "Particle.h"
#include "GlobalAsteroidVariables.h"
#include <list>
#include <memory>

using namespace std;

class Explosion: public GameObject
{

public:
	Explosion(float x, float y, float xVel, float yVel, float red, float green, float blue);
	void draw();
	int life;
private:
	float red;
	float green;
	float blue;
	list<shared_ptr<GameObject>> particles;
};

#endif