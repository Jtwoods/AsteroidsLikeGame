#include "Life.h"

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include "GlobalAsteroidVariables.h"
#include <math.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gl\freeglut.h>
#include "GameObject.h"

Life::Life(int pos)
{

	//Set initial angular velocity.

	angle = 0.0f;
	angleX = 0.0f;
	angleY = 0.0f;
	angleZ = 0.0f;
	angularVelocityX = 0.0f;
	angularVelocityY = 0.3f;
	angularVelocityZ = 0.0f;

	x = -15.0 + pos*.6;
	y = 15.0f;
	z = 2.0f;
	velocity = 0.0f;
	velocityX = 0.0f;
	velocityY = 0.0f;
	velocityZ = 0.0f;
	acceleration = 0.0f;
	MaxAccel = 0.01f;
	MaxVelocity = 0.35f;

	init();
}

void Life::init()
{
	GLfloat material[] = {63/255.0, 69/255.0, 101/255.0, 0 };
	GLfloat shine[] = {2};

	name = glGenLists(1);
	glNewList(name,GL_COMPILE);

	//Set the color for the lit player ship.
	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	glMaterialfv( GL_FRONT, GL_SHININESS, shine);
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutSolidCone(LIFE_SIDE, LIFE_FRONT, 12, 4);
	glPushMatrix();
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glutSolidCone(LIFE_SIDE, LIFE_BACK, 12, 2);
	glPopMatrix();
	glPopMatrix();
	glEndList();
}

void Life::draw()
{	

		glPushMatrix();
		glTranslatef(x, y, z);
		glPushMatrix();
		glRotatef(angle,angleX,angleY,angleZ);

		glCallList(name);
		
		glPopMatrix();
		glPopMatrix();
		
}