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
	bool GetAlive();
	void KillAlienShip();
	bool GetIsSmall();
	void SetIsSmall(bool is_small_now);
	float GetHeight();
	void SetHeight(float h);
	float GetWidth();
	void SetWidth(float w);
	float GetShotVelocity();
	void SetShotVelocity(float velocity);
	int GetPSubP();
	void SetPSubP(int p);
	int GetPSubA();
	void SetPSubA(int a);
	int GetPSubR();
	void SetPSubR(int r);

private:
	void init();
	unsigned int name;
	long time;
	bool alive;
	bool dead;
	bool is_small;
	float height;
	float width;
	float shotVelocity;
	int p_p;
	int p_a;
	int p_r;
};

#endif