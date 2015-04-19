#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Score.h"
#include "GameObject.h"
#include "Stars.h"
#include <memory>
#include <list>

using namespace std;

class BackGround
{
public:
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void makeShot() = 0;
	bool done;
	bool game_over;
	bool safe;
	int current;
	long count;
	int lives;
	list<shared_ptr<GameObject>> player_lives;
	Score score;
	shared_ptr<Stars> stars;
};

#endif