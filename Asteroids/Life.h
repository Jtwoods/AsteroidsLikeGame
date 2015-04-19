#pragma once
#ifndef LIFE_H
#define LIFE_H
#include "GameObject.h"

using namespace std;

class Life: public GameObject
{
public:
	Life(int pos);
	void draw();
private:
	void init();
	unsigned int name;
};

#endif