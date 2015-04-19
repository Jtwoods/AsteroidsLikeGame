#include "Shot.h"
#include "GameObject.h"

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include <iostream>
#include <math.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glut.h>
#include "GlobalAsteroidVariables.h"

using namespace std;


Shot::Shot(float X, float Y, float xVel, float yVel, float r)
{
	x = X;
	y = Y;
	z = 0.0f;
	velocityX = xVel;
	velocityY = yVel;
	angle = r;
	velocity = 0.0f;
	velocityZ = 0.0f;
	acceleration = 0.0f;
	MaxAccel = 0.00f;
	MaxVelocity = 4.0f;

	//Set initial angular velocity and degrees.
	angle = 0.0f;
	angleX = 0.0f;
	angleY = 0.0f;
	angleZ = 0.0f;
	angularVelocityX = 0.0f;
	angularVelocityY = 0.0f;
	angularVelocityZ = 0.0f;

	//Start the lifespan timer.
	lifeCount = 0;

}

void Shot::draw()
{

	GLfloat ambientMaterial[] = {63/255.0, 69/255.0, 101/255.0, 0 };
	GLfloat shine[] = {2};
	GLfloat reflect[] = {70/255.0, 91/255.0, 159/255.0, 0 };
	GLfloat none[] = {0,0,0};


	//Set the color for the lit asteroid.
	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambientMaterial);
	glMaterialfv( GL_FRONT, GL_SHININESS, shine);

	glMaterialfv( GL_FRONT, GL_EMISSION, reflect);

	size = 5.0f;

	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(SHOT_RADIUS,7,7);
	glEnd();
	glPopMatrix();

	glMaterialfv( GL_FRONT, GL_EMISSION, none);
}