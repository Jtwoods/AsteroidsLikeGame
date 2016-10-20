#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include "Asteroid.h"
#include "AlienSpaceShip.h"
#include "PlayerSpaceShip.h"
#include "GameObject.h"
#include "GlobalAsteroidVariables.h"
#include "BackGround.h"
#include "Level.h"
#include "Intro.h"
#include "Stars.h"
#include <time.h> 
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gl\freeglut.h>
#include <math.h>
#include <iostream>

using namespace std;
/*
/Author: James Woods
/Asteroids builds on the functionality of the GLUT_DEMO.C( 12-Jun-98	Christopher G. Healey	Initial implementation).
/In addition to the 3D viewer provided in the demo, I have added an asteroid,
/player spaceship, and an alien spaceship.
*/

  //  Module global prototypes

#define TIMER_DELAY 1000/60

void  display_obj( void );
void  handle_menu( int );
void  handle_rotation( int );
void  handle_mouse( int, int, int, int );

	//  Module global variables

	//These are the objects used to display the space craft and asteroid.
	shared_ptr<BackGround> level;
	shared_ptr<PlayerSpaceShip> player;

	
int    btn[ 3 ] = { 0 };		// Current button state
int    mouse_x, mouse_y;		// Current mouse position

//Booleans to track the left and right key presses.
bool leftPressed;
bool rightPressed;
bool firing;

void handle_rotation(int n)

{
	//Update the current level.
	level->update();

	if(level->done)
	{
		unsigned int toTransfer = level->stars->getName();

		//Create a level with a random number of asteroids greater than the number in the last.
		level = make_shared<Level>((rand() % MAX_SPAWN_ASTEROIDS) + level->current, player, level->player_lives.size(), level->score.getScore());
		level->stars = make_shared<Stars>(toTransfer);
	}
	

	//Mark the screen for an update.
	glutPostRedisplay();
}					// End routine handle_rotation


void display_obj( void )

  //  This routine displays the space ships and asteroids.
{

  glMatrixMode( GL_MODELVIEW );		// Setup model transformations
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  
  //Display as lines.
  glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);

  //Cull the back face.
  glEnable( GL_CULL_FACE ); glCullFace( GL_BACK );

  //Draw the current state of the level.
  level->draw();
  glutTimerFunc(TIMER_DELAY,handle_rotation,0);

  glFlush();				// Flush OpenGL queue

  glutSwapBuffers();			// Display back buffer
}					// End routine display_obj


void handle_menu( int ID )

  //  This routine handles popup menu selections
  //	Modified from GLUT_DEMO.C.
  //  ID:  Menu entry ID
{
  switch( ID ) {
	case 0:			// Quit
		exit( 0 );
	break;
	case 1:
	{
		player = make_shared<PlayerSpaceShip>();
		unsigned int toTransfer = level->stars->getName();
		level = make_shared<Level>((rand() % MAX_NEW_ASTEROIDS) + 1, player, 3, 0);
		level->stars = make_shared<Stars>(toTransfer);
	}
  }
  

}					// End routine handle_menu

void handle_mouse( int b, int s, int x, int y )

	//From GLUT_DEMO.C
  //  This routine acts as a callback for GLUT mouse events
  // 
  //  b:     Mouse button (left, middle, or right)
  //  s:     State (button down or button up)
  //  x, y:  Cursor position
{
  if ( s == GLUT_DOWN ) {		// Store button state if mouse down
    btn[ b ] = 1;
  } else {
    btn[ b ] = 0;
  }

  mouse_x = x;
  mouse_y = glutGet( GLUT_WINDOW_HEIGHT ) - y;
}					// End routine handle_mouse

void handle_left_right(int key, int i, int j)
{ 
	switch (key) {
		case GLUT_KEY_LEFT:
			leftPressed = true;
			if (!rightPressed) {
				player->angularVelocityZ = player->ANGULAR_MAX;
			}
		break;
		case GLUT_KEY_RIGHT:
			rightPressed = true;
			if (!leftPressed) {
				player->angularVelocityZ = -player->ANGULAR_MAX;
			}
		break;
	}
}
void handle_left_right_release(int key, int i, int j)
{
	switch (key) {
		case GLUT_KEY_LEFT:
			leftPressed = false;
		break;
		case GLUT_KEY_RIGHT:
			rightPressed = false;
		 break;
	}
	player->angularVelocityZ = 0.0f;
}

void handle_key(unsigned char key, int i, int j)
{
		switch (key) {
		case 'x':
			player->acceleration = player->MaxAccel;
			player->moving = true;
		break;
		case 'z':
			if(!firing)
				level->makeShot();
			firing = true;
		break;
	}
}

void handle_key_up(unsigned char key, int i, int j)
{
	switch (key) {
		case 'x':
			player->acceleration = 0.0f;
			player->moving = false;
		break;
		case 'z':
			firing = false;
		break;
	}
}

void main( int argc, char *argv[] )
{
	//initialize data.
	leftPressed = false;
	rightPressed = false;
	firing = false;

	srand(time(NULL));

  glutInit( &argc, argv );		// Initialize GLUT
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );


  glutCreateWindow( "Asteroids Part 2" );


  	//Initialize the space craft and the initial level.
	player = make_shared<PlayerSpaceShip>();
	level = make_shared<Intro>();

  glutDisplayFunc( display_obj );	// Setup GLUT callbacks
  
  glutMouseFunc( handle_mouse );

  //Handle left and right keys.
  glutSpecialFunc(handle_left_right);
  glutSpecialUpFunc(handle_left_right_release);

  //Handle z and x keys.
  glutKeyboardFunc(handle_key);
  glutKeyboardUpFunc(handle_key_up);

  glutCreateMenu( handle_menu );	// Setup GLUT popup menu with quit option.
  glutAddMenuEntry( "Quit", 0 );
  glutAddMenuEntry( "New Game", 1);
  glutAttachMenu( GLUT_RIGHT_BUTTON );

  glMatrixMode( GL_PROJECTION );	// Setup perspective projection
  glLoadIdentity();
  gluPerspective( 70, 1, 1, 35 );

  glMatrixMode( GL_MODELVIEW );		// Setup model transformations
  glLoadIdentity();
  gluLookAt( 0, 0, 25, 0, 0, -1, 0, 1, 0 );

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);


  //FROM lighting.cpp as provided in the course description.
    
  float  amb[] = { 0, 0, 0, 1 };	// Ambient material property
  float  lt_amb[] = { .2, .2, .2, 1 };	// Ambient light property
  float  lt_dif[] = { .8, .8, .8, 1 };	// Ambient light property
  float  lt_pos[] = {			// Light position
           0, .39392, .91914, 0
         };
  float  lt_spc[] = { 0, 0, 0, 1 };	// Specular light property


  //  Set default ambient light in scene

  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb );

  //  Set Light 0 position, ambient, diffuse, specular intensities

  glLightfv( GL_LIGHT0, GL_POSITION, lt_pos );
  glLightfv( GL_LIGHT0, GL_AMBIENT, lt_amb );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, lt_dif );
  glLightfv( GL_LIGHT0, GL_SPECULAR, lt_spc );

  //  Enable Light 0 and GL lighting

  glEnable( GL_LIGHT0 );
  glEnable( GL_LIGHTING );

  glShadeModel( GL_FLAT );		// Flat shading
  glEnable( GL_NORMALIZE );		// Normalize normals

  //End of code from lighting.cpp

  glClearDepth( 1.0 );			// Setup background colour
  glEnable(GL_DEPTH);
  glClearColor( 0, 0, 0, 0 );
  glEnable( GL_DEPTH_TEST );

  glutTimerFunc(TIMER_DELAY,handle_rotation,0);
  glutMainLoop();			// Enter GLUT main loop


}					// End mainline