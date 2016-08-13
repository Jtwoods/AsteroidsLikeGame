#include "Stars.h"

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <gl\freeglut.h>

#include "GlobalAsteroidVariables.h"

Stars::Stars(int nam)
{
	name = nam;
}

Stars::Stars()
{
	name = glGenLists(1);
	glNewList(name,GL_COMPILE);

	GLfloat material[] = {1, 1, 1, 0};
	GLfloat reflect[] = {1, 1, 1, 0 };
	GLfloat none[] = {0,0,0};
	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	glMaterialfv( GL_FRONT, GL_EMISSION, reflect);

	//Set the color for the lit player ship.
	for(int i = 0; i < NUMBER_STARS; i++)
	{
		glPushMatrix();
		glTranslatef(fRand(-STAR_WIDTH, STAR_WIDTH), fRand(-STAR_HEIGHT, STAR_HEIGHT), -fRand(STAR_MIN_Z, STAR_MAX_Z));
		glutSolidSphere(.02f,12,12);
		//glPushMatrix();

		////glEnable(GL_BLEND);
		////glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		////glutSolidSphere(0.4f, 6, 6);
		////glDisable(GL_BLEND);
		//glPopMatrix();
		glPopMatrix();
	}
	glMaterialfv( GL_FRONT, GL_EMISSION, none);
	glEndList();
}

void Stars::draw()
{
	glCallList(name);
}

unsigned int Stars::getName()
{
	return name;
}