#pragma once
#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "GameObject.h"
#include "PlayerSpaceShip.h"
#include "AlienSpaceShip.h"
#include <memory>

using namespace std;

class Movement
{
public:
	void clip(shared_ptr<GameObject> object);
	void clip(shared_ptr<PlayerSpaceShip> object);
	void rotate(shared_ptr<GameObject> object);
	void rotate(shared_ptr<PlayerSpaceShip> object);
	void move(shared_ptr<GameObject> object);
	void move(shared_ptr<PlayerSpaceShip> object);
	void move(shared_ptr<AlienSpaceShip> object);
};

#endif
