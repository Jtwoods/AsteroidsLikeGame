#pragma once
#ifndef INTRO_H
#define INTRO_H
#include "GameObject.h"
#include "BackGround.h"

using namespace std;

class Intro: public BackGround
{
public:
	Intro();
	void draw();
	void update();
	void makeShot();
};

#endif