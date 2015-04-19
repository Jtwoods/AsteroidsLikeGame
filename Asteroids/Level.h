#pragma once
#ifndef LEVEL_H
#define LEVEL_H
#include "GameObject.h"
#include "Collisions.h"
#include "PlayerSpaceShip.h"
#include "Explosion.h"
#include "AlienSpaceShip.h"
#include "BackGround.h"
#include "Life.h"
#include "Movement.h"
#include "Score.h"
#include "Shot.h"
#include <memory>
#include <list>

using namespace std;

class Level: public BackGround
{
public:
	Level();
	Level(int n, shared_ptr<PlayerSpaceShip> player, int lives_to_start, long score_to_start);
	~Level();
	void draw();
	void update();
	void removeLife();
	void makeShot();
private:
	list<shared_ptr<GameObject>> objects;
	list<shared_ptr<Shot>> pshots;
	list<shared_ptr<Shot>> ashots;
	list<shared_ptr<Explosion>> explosions;
	shared_ptr<PlayerSpaceShip> playerShip;
	shared_ptr<AlienSpaceShip> alienShip;
	Movement mover;
	Collisions collider;
	int deadCount;
	void makeAlienShot();
	float getAngle(float x, float y);
};

#endif