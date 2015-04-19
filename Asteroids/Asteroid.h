#pragma once
#ifndef ASTEROID_H
#define ASTEROID_H
#include "GameObject.h"
#include <list>
#include <memory>

using namespace std;

class Asteroid: public GameObject
{
	
public:
	Asteroid();
	Asteroid(float rad);
	~Asteroid();
	virtual void draw();
	list<shared_ptr<GameObject>> split(float xVel, float yVel); 
	float radius;

private:
	int Faces[960];
	float Verts[486];
	float fRandNonZero(float fmin, float fmax);
	void init();
	void init_arrays();
	int name;
};

#endif