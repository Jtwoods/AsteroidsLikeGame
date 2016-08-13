#include "Score.h"

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include "GlobalAsteroidVariables.h"
#include <iostream>
#include <math.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gl\freeglut.h>
#include <sstream>
#include <iostream>

using namespace std;

Score::Score()
{
	score = 0;
	toBonus = BONUS;
}

Score::Score(long count)
{
	score = count;
	toBonus = BONUS - count%BONUS;
}

bool Score::addScore(long num)
{
	bool toReturn = false;
	score += num;
	toBonus -= num;

	if(toBonus <= 0)
	{
		toReturn = true;
		toBonus = BONUS;
	}

	return toReturn;
}

long Score::getScore()
{
	return score;
}


void Score::draw()
{
	//Convert the long score to a string.
	std::stringstream stream;
	stream << score;
	string value = stream.str();
	//Get the length of the string.
	int length = value.size();
	float size = 0;

	//Use Glut to display the string.

	GLfloat material[] = {63/255.0, 69/255.0, 101/255.0, 0 };
	GLfloat shine[] = {2};
	GLfloat reflect[] = {26/255.0, 39/255.0, 105/255.0, 0 };
	GLfloat none[] = {0,0,0};

	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	glMaterialfv( GL_FRONT, GL_SHININESS, shine);
	glMaterialfv( GL_FRONT, GL_EMISSION, reflect);

	for (int i = 0; i < length; i++) 
	{
		size += glutStrokeWidth(GLUT_STROKE_ROMAN, value[i]);
	}

	glPushMatrix();
	glTranslatef(15 - size/152.38, 14.5, 2);
	glScalef(1/152.38, 1/152.38, 1/152.38);
	for (int i = 0; i < length; i++) 
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, value[i]);
	}

	glMaterialfv( GL_FRONT, GL_EMISSION, none);
	glPopMatrix();
}
