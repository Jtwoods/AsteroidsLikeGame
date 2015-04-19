#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
#include "GameObject.h"

class Particle: public GameObject
{

public:
	Particle(float x, float y, float xVel, float yVel, float red, float green, float blue);
	void draw();
	float size;
private:
	float red;
	float green;
	float blue;
	int name;
};

#endif