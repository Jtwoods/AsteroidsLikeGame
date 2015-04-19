#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject
{
public:
	virtual void draw() = 0;
	float angle;
	float angularVelocityX;
	float angularVelocityY;
	float angularVelocityZ;
	float angleX;
	float angleY;
	float angleZ;
	float velocity;
	float velocityX;
	float velocityY;
	float velocityZ;
	float x;
	float y;
	float z;
	float acceleration;
	float MaxAccel;
	float MaxVelocity;
	int count;
	long points;
};

#endif