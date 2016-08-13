#include "ShotParticle.h"
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
#include <gl\freeglut.h>

using namespace std;


ShotParticle::ShotParticle(float X, float Y, float xVel, float yVel)
{
	x = X;
	y = Y;

	red = 63/255.0;
	green = 69/255.0;
	blue = 101/255.0;

	//Set initial angular velocity and degrees.
	z = 0.0f;
	velocity = 0.0f;

	if(xVel == 0)
	{
		velocityX = fRand(-PARTICLE_VELOCITY_RANGE, PARTICLE_VELOCITY_RANGE);
		velocityY = 0;
	} 
	else
	{
			if(yVel == 0)
			{
				velocityY = fRand(-PARTICLE_VELOCITY_RANGE, PARTICLE_VELOCITY_RANGE);
				velocityX = 0;
			}
			else
			{
				float X_s = fRand(-180, 180);
				if(X_s == 0)
					X_s = 1.1;
				float Y_s = -(xVel*(X_s - x))/yVel + y;
				velocityX = PARTICLE_VELOCITY_RANGE*(X_s - x);
				velocityY = PARTICLE_VELOCITY_RANGE*(Y_s - y);
			}
	}


	velocityZ = fRand(-PARTICLE_VELOCITY_RANGE, PARTICLE_VELOCITY_RANGE);

	float mag = sqrt(velocityX*velocityX + velocityY*velocityY);
	float vel = fRand(-PARTICLE_VELOCITY_RANGE, PARTICLE_VELOCITY_RANGE);

	velocityX = (velocityX/mag)*vel;
	velocityY = (velocityY/mag)*vel;

	mag = sqrt(velocityX*velocityX + velocityY*velocityY + velocityZ*velocityZ);

	if(rand() % 3 == 0)
		vel = fRandNonZero(OUTER_MIN_PARTICLE_VELOCITY, OUTER_MAX_PARTICLE_VELOCITY);
	else
		vel = fRandNonZero(INNER_MIN_PARTICLE_VELOCITY, INNER_MAX_PARTICLE_VELOCITY);

	velocityX = (velocityX/mag)*vel;
	velocityY = (velocityY/mag)*vel;
	velocityZ = (velocityZ/mag)*vel;


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
	size = .1f;
}

//Creates a random positive or negative float value between the given min and max values.
float ShotParticle::fRandNonZero(float fMin, float fMax)
{
	int sign = -1;
	int decider = rand() % 2;
	if (decider == 1)
		sign = 1;
    float f = (float)rand() / RAND_MAX;
    return sign*(fMin + f * (fMax - fMin));
}

void ShotParticle::draw()
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