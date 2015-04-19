#pragma once
#ifndef SHOTPARTICLE_H
#define SHOTPARTICLE_H
#include "GameObject.h"

class ShotParticle: public GameObject
{

public:
	ShotParticle(float x, float y, float xVel, float yVel);
	void draw();
	float size;
private:
float fRandNonZero(float min, float max);
	float red;
	float green;
	float blue;
	int name;
};

#endif