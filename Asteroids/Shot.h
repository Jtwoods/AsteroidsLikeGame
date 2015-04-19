#pragma once
#ifndef SHOT_H
#define SHOT_H
#include "GameObject.h"

class Shot: public GameObject
{

public:
	Shot(float x, float y, float xVel, float yVel, float r);
	void draw();
	int lifeCount;
	float size;
};

#endif