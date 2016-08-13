#include "PlayerSpaceShip.h"

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
#include "GameObject.h"

using namespace std;

PlayerSpaceShip::PlayerSpaceShip()
{

	//Set initial angular velocity.
	moving = false;
	rotating = false;
	alive = true;

	angle = 0.0f;
	angleX = 0.0f;
	angleY = 0.0f;
	angleZ = 0.0f;
	angularVelocityX = 0.0f;
	angularVelocityY = 0.0f;
	angularVelocityZ = 0.0f;

	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	velocity = 0.0f;
	velocityX = 0.0f;
	velocityY = 0.0f;
	velocityZ = 0.0f;
	acceleration = 0.0f;
	MaxAccel = 0.0065f;
	MaxVelocity = 0.20f;
	ANGULAR_MAX = 4.0f;
	shotVelocity = 0.29f;

	init();
}

void PlayerSpaceShip::init()
{

	name = glGenLists(1);
	glNewList(name,GL_COMPILE);

	//Set the color for the lit player ship.
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutSolidCone(SHIP_SIDE, SHIP_FRONT, 12, 4);
	glPushMatrix();
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glutSolidCone(SHIP_SIDE, SHIP_BACK, 12, 2);
	glPopMatrix();
	glPopMatrix();
	glEndList();
}

void PlayerSpaceShip::draw()
{	
	if(alive) 
	{
		GLfloat material[] = {63/255.0, 69/255.0, 101/255.0, 0 };
		GLfloat shine[] = {2};
		GLfloat reflect[] = {26/255.0, 39/255.0, 105/255.0, 0 };
		GLfloat none[] = {0,0,0};

		glPushMatrix();
		glTranslatef(x, y, z);
		glPushMatrix();
		glRotatef(angle,angleX,angleY,angleZ);

		//Set the color for the lit player ship.
		glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
		glMaterialfv( GL_FRONT, GL_SHININESS, shine);

		//If the player is accelerating we will light up the back
		//of the ship. If not we can use our list to render the ship.
		if(moving && rand()%3 == 0)
		{
			//glMaterialfv( GL_FRONT, GL_EMISSION, reflect);
			glPushMatrix();
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glutSolidCone(.4, 1.0, 12, 4);
			glPushMatrix();
			glRotatef(180.0, 1.0, 0.0, 0.0);
		
			glMaterialfv( GL_FRONT, GL_EMISSION, reflect);
				
	
			glutSolidCone(.4, .5, 12, 2);
			glPopMatrix();
			glPopMatrix();
			glMaterialfv( GL_FRONT, GL_EMISSION, none);

		}
		else
		{
			glCallList(name);
		}
		glPopMatrix();
		glPopMatrix();
	}	
}