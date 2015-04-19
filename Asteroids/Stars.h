#pragma once
#ifndef STARS_H
#define STARS_H
#include "GameObject.h"

using namespace std;

class Stars
{
public:
	Stars(int nam);	
	Stars();
	void draw();
	unsigned int getName();
private:
	unsigned int name;
};

#endif                                                                                                                                                                                                                                                                    