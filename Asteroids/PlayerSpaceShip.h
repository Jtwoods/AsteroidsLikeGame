#pragma once
#ifndef PLAYERSPACESHIP_H
#define PLAYERSPACESHIP_H
#include "GameObject.h"

class PlayerSpaceShip: public GameObject
{
	
public:
	PlayerSpaceShip();
	void draw();
	bool moving;
	bool rotating;
	float shotVelocity;
	float ANGULAR_MAX;  
	bool alive;

private:
	unsigned int name;
	void init();

};

#endif