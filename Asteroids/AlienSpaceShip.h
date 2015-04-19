#pragma once
#ifndef ALIENSPACESHIP_H
#define ALIENSPACESHIP_H
#include "GameObject.h"

class AlienSpaceShip: public GameObject
{
	
public:
	AlienSpaceShip();
	AlienSpaceShip(bool is_small_now);
	void update(long count, long points, int asteroids);
	void draw();
	bool alive;
	bool dead;
	bool is_small;
	float height;
	float width;
	float shotVelocity;
	int p_p;
	int p_a;
	int p_r;
private:
	void init();
	unsigned int name;
	//float fRand(float fMin, float fMax);
	long time;
};

#endif