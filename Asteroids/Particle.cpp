#include "Particle.h"
#include "GameObject.h"

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
#include <glut.h>

using namespace std;


Particle::Particle(float X, float Y, float xVel, float yVel, float redValue, float greenValue, float blueValue)
{
	x = X;
	y = Y;

	red = redValue;
	green = greenValue;
	blue = blueValue;

	//Set initial angular velocity and degrees.
	z = 0.0f;
	velocity = 0.0f;
	velocityX = fRand(-PARTICLE_VELOCITY_RANGE, PARTICLE_VELOCITY_RANGE) + xVel;
	velocityY = fRand(-PARTICLE_VELOCITY_RANGE, PARTICLE_VELOCITY_RANGE) + yVel;
	velocityZ = fRand(-PARTICLE_VELOCITY_RANGE, PARTICLE_VELOCITY_RANGE);
	acceleration = 0.0f;
	MaxAccel = 0.01f;
	MaxVelocity = 2.0f;

	//Set initial angular velocity and degrees to random values.
	angle = fRand(-.9f, .9f);
	angleX = fRand(-.9f, .9f);
	angleY = fRand(-.9f, .9f);
	angleZ = fRand(-.9f, .9f);
	angularVelocityX = fRand(-1.3f, 1.3f);
	angularVelocityY = fRand(-1.3f, 1.3f);
	angularVelocityZ = fRand(-1.3f, 1.3f);
	size = .22f;
}


void Particle::draw()
{

	//Build the colors for the particle.
	float alpha = 1 - count/MAX_EXPLOSION_LIFE;

	GLfloat ambientMaterial[] = {red, green, blue, alpha };
	GLfloat difuseMaterial[] = {red, green, blue, alpha };
	GLfloat shine[] = {.03};


	glPushMatrix();
	glTranslatef(x, y, z);
	glPushMatrix();
	glRotatef(angle,angleX,angleY,angleZ);
	glScalef(size, size, size);

	//Set the color for the particle.
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv( GL_FRONT, GL_DIFFUSE, difuseMaterial);
	glMaterialfv( GL_FRONT, GL_SHININESS, shine);

	glBegin(GL_POLYGON);

	glNormal3f(0, 0, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 1, 0);

	glEnd();


	glPopMatrix();
	glPopMatrix();
}