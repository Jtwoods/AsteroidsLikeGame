#include "Asteroid.h"

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
#include <cstdlib>
#include "GameObject.h"
#include <list>
#include <memory>

using namespace std;

Asteroid::Asteroid()
{
	radius = 1;
	points = LARGE_ASTEROID_POINTS;
	//Initialize the vertex and face arrays.
	init_arrays();

	//Set initial angular velocity and degrees to random values.
	angle = 0.0f;
	angleX = 0.0f;
	angleY = 0.0f;
	angleZ = 0.0f;
	angularVelocityX = fRand(-.9f, .9f);
	angularVelocityY = fRand(-.9f, .9f);
	angularVelocityZ = fRand(-.9f, .9f);

	//Set a random position on an edge and initialize all movement parameters.
	//Velocity will be random.
	int sideChoice = rand() % 4;

	switch(sideChoice) 
	{
		case 0:
			x = -WIDTH;
			y = fRand(-HEIGHT, HEIGHT);
		break;
		case 1:
			x = WIDTH;
			y = fRand(-HEIGHT, HEIGHT);
		break;
		case 2:
			x = fRand(-WIDTH, WIDTH);
			y = -HEIGHT;
		break;
		case 3:
			x = fRand(-WIDTH, WIDTH);
			y = HEIGHT;
		break;
	}
		
	z = 0.0f;
	velocity = 0.0f;
	velocityX = fRandNonZero(-ASTEROID_VELOCITY_RANGE, ASTEROID_VELOCITY_RANGE);
	velocityY = fRand(-ASTEROID_VELOCITY_RANGE, ASTEROID_VELOCITY_RANGE);

	while(velocityX <= .01 || velocityY <= .01)
	{
		velocityX = fRand(-ASTEROID_VELOCITY_RANGE, ASTEROID_VELOCITY_RANGE);
		velocityY = fRand(-ASTEROID_VELOCITY_RANGE, ASTEROID_VELOCITY_RANGE);
	}

	velocityZ = 0.0f;
	acceleration = 0.0f;
	MaxAccel = 0.01f;
	MaxVelocity = 2.0f;

	init();
}

Asteroid::Asteroid(float rad)
{
	radius = rad;
	points = LARGE_ASTEROID_POINTS;

		//Initialize the vertex and face arrays.
	init_arrays();

	//Set initial angular velocity and degrees to random values.
	angle = 0.0f;
	angleX = 0.0f;
	angleY = 0.0f;
	angleZ = 0.0f;
	angularVelocityX = fRand(-.9f, .9f);
	angularVelocityY = fRand(-.9f, .9f);
	angularVelocityZ = fRand(-.9f, .9f);

	//Set a random position on an edge and initialize all movement parameters.
	//Velocity will be random.
	int sideChoice = rand() % 4;

	switch(sideChoice) 
	{
		case 0:
			x = -WIDTH;
			y = fRand(-HEIGHT, HEIGHT);
		break;
		case 1:
			x = WIDTH;
			y = fRand(-HEIGHT, HEIGHT);
		break;
		case 2:
			x = fRand(-WIDTH, WIDTH);
			y = -HEIGHT;
		break;
		case 3:
			x = fRand(-WIDTH, WIDTH);
			y = HEIGHT;
		break;
	}
		
	z = 0.0f;
	velocity = 0.0f;
	velocityX = fRandNonZero(ASTEROID_MIN_VEL, ASTEROID_VELOCITY_RANGE);
	velocityY = fRandNonZero(ASTEROID_MIN_VEL, ASTEROID_VELOCITY_RANGE);
	velocityZ = 0.0f;
	acceleration = 0.0f;
	MaxAccel = 0.01f;
	MaxVelocity = 2.0f;

	init();
}

Asteroid::~Asteroid()
{

}

//Returns a list of smaller asteroids to replace this one when it is shot or an
//empty list if this asteroid can not be split.
list<shared_ptr<GameObject>> Asteroid::split(float xVel, float yVel)
{
	list<shared_ptr<GameObject>> toReturn = list<shared_ptr<GameObject>>();

	//If this asteroid radius is bigger than the min create two asteroids to replace it.
	if(radius > MIN_RADIUS) 
	{
		//Build the new asteroids.
		shared_ptr<GameObject> one = make_shared<Asteroid>(radius - .2f);
		shared_ptr<GameObject> two = make_shared<Asteroid>(radius - .2f);

		one->points = (int)points*2;
		two->points = (int)points*2;
		if(abs(radius - 1) < 0.01)
		{
			one->points += (int)points/2;
			two->points += (int)points/2;
		}

		//Set new random velocity direction.
		velocityX = fRandNonZero(ASTEROID_SPLIT_VEL, ASTEROID_VELOCITY_RANGE);
		velocityZ = fRandNonZero(ASTEROID_SPLIT_VEL, ASTEROID_VELOCITY_RANGE);

		float xOne = 0;
		float yOne = 0;
		float xTwo = 0;
		float yTwo = 0;

		float xVelOne = 0;
		float yVelOne = 0;
		float xVelTwo = 0;
		float yVelTwo = 0;

		float magShot = (float)sqrt(yVel*yVel + xVel*xVel);
		float magAst = (float)sqrt(velocityY*velocityY + velocityX*velocityX);

		//Calculate the vectors for velocity.
		xVelOne = (velocityX/magAst - (yVel/magShot))*magAst*.7f;
		yVelOne = (velocityY/magAst + (xVel/magShot))*magAst*.7f;

		xVelTwo = (velocityX/magAst + (yVel/magShot))*magAst*.7f;
		yVelTwo = (velocityY/magAst - (xVel/magShot))*magAst*.7f;
		

		//Set velocity.
		one->velocityX = xVelOne;
		one->velocityY = yVelOne;
		two->velocityX = xVelTwo;
		two->velocityY = yVelTwo;

		//Magnitude of the shot can not equal zero.
		if(magShot == 0)
		{
			magShot = 1;
		}

		//Calculate position.
		xOne = x - yVel/magShot*radius;
		yOne = y + xVel/magShot*radius;
		xTwo = x + yVel/magShot*radius;
		yTwo = y - xVel/magShot*radius;

		//Set position.
		one->x = xOne;
		one->y = yOne;
		two->x = xTwo;
		two->y = yTwo;

		toReturn.push_front(one);
		toReturn.push_front(two);
	}
	
	return toReturn;
}

//Creates a random positive or negative float value between the given min and max values.
float Asteroid::fRandNonZero(float fMin, float fMax)
{
	int sign = -1;
	int decider = rand() % 2;
	if (decider == 1)
		sign = 1;
    float f = (float)rand() / RAND_MAX;
    return sign*(fMin + f * (fMax - fMin));
}


void Asteroid::init()
{
	name = glGenLists(1);
	glNewList(name, GL_COMPILE);

	//Declare the normal for the polygob and draw the lines coresponding to the faces of each triangle using the vertex coordinates.
	for(int i = 0; i < sizeof(Faces)/sizeof(Faces[0]); i += 3)
	{
		
		glBegin(GL_POLYGON);

		//Calculate the normal for the face.
		float nz, ny, nx = 0.0f;
		float dz, dy, dx = 0.0f;
		float donez, doney, donex = 0.0f;

		dx = Verts[(Faces[i + 1] - 1)*3 + 0] - Verts[(Faces[i] - 1)*3 + 0];
		dy = Verts[(Faces[i + 1] - 1)*3 + 1] - Verts[(Faces[i] - 1)*3 + 1];
		dz = Verts[(Faces[i + 1] - 1)*3 + 2] -  Verts[(Faces[i] - 1)*3 + 2];

		donex = Verts[(Faces[i + 2] - 1)*3 + 0] - Verts[(Faces[i + 1] - 1)*3 + 0];
		doney = Verts[(Faces[i + 2] - 1)*3 + 1] - Verts[(Faces[i + 1] - 1)*3 + 1];
		donez = Verts[(Faces[i + 2] - 1)*3 + 2] - Verts[(Faces[i + 1] - 1)*3 + 2];


		nx = dy*donez - dz*doney;
		ny = dz*donex - dx*donez;
		nz = dx*doney - dy*donex;


		//record the normal with openGL
		glNormal3f( nx, ny, nz );


		glVertex3f(Verts[(Faces[i] - 1)*3 + 0],Verts[(Faces[i] - 1)*3 + 1], Verts[(Faces[i] - 1)*3 + 2]);
		glVertex3f(Verts[(Faces[i + 1] - 1)*3 + 0],Verts[(Faces[i + 1] - 1)*3 + 1], Verts[(Faces[i + 1] - 1)*3 + 2]);
		glVertex3f(Verts[(Faces[i + 2] - 1)*3 + 0],Verts[(Faces[i + 2] - 1)*3 + 1], Verts[(Faces[i + 2] - 1)*3 + 2]);
		glEnd();
	}
	glEndList();
}


void Asteroid::init_arrays()
{

		//From: http://prideout.net/blog/?p=48, modified from original.

	    int tempFaces[] = { 1, 13, 21,
2, 15, 27,
1, 21, 33,
1, 33, 39,
1, 39, 22,
2, 27, 48,
3, 18, 54,
4, 30, 60,
5, 36, 66,
6, 42, 72,
2, 48, 49,
3, 54, 55,
4, 60, 61,
5, 66, 67,
6, 72, 43,
7, 75, 93,
8, 78, 96,
9, 81, 99,
10, 84, 102,
11, 87, 88,
19, 18, 3,
20, 103, 19,
21, 104, 20,
19, 103, 18,
103, 17, 18,
20, 104, 103,
104, 105, 103,
103, 105, 17,
105, 16, 17,
21, 13, 104,
13, 14, 104,
104, 14, 105,
14, 15, 105,
105, 15, 16,
15, 2, 16,
25, 24, 6,
26, 106, 25,
27, 107, 26,
25, 106, 24,
106, 23, 24,
26, 107, 106,
107, 108, 106,
106, 108, 23,
108, 22, 23,
27, 15, 107,
15, 14, 107,
107, 14, 108,
14, 13, 108,
108, 13, 22,
13, 1, 22,
31, 30, 4,
32, 109, 31,
33, 110, 32,
31, 109, 30,
109, 29, 30,
32, 110, 109,
110, 111, 109,
109, 111, 29,
111, 28, 29,
33, 21, 110,
21, 20, 110,
110, 20, 111,
20, 19, 111,
111, 19, 28,
19, 3, 28,
37, 36, 5,
38, 112, 37,
39, 113, 38,
37, 112, 36,
112, 35, 36,
38, 113, 112,
113, 114, 112,
112, 114, 35,
114, 34, 35,
39, 33, 113,
33, 32, 113,
113, 32, 114,
32, 31, 114,
114, 31, 34,
31, 4, 34,
24, 42, 6,
23, 115, 24,
22, 116, 23,
24, 115, 42,
115, 41, 42,
23, 116, 115,
116, 117, 115,
115, 117, 41,
117, 40, 41,
22, 39, 116,
39, 38, 116,
116, 38, 117,
38, 37, 117,
117, 37, 40,
37, 5, 40,
46, 45, 11,
47, 118, 46,
48, 119, 47,
46, 118, 45,
118, 44, 45,
47, 119, 118,
119, 120, 118,
118, 120, 44,
120, 43, 44,
48, 27, 119,
27, 26, 119,
119, 26, 120,
26, 25, 120,
120, 25, 43,
25, 6, 43,
52, 51, 7,
53, 121, 52,
54, 122, 53,
52, 121, 51,
121, 50, 51,
53, 122, 121,
122, 123, 121,
121, 123, 50,
123, 49, 50,
54, 18, 122,
18, 17, 122,
122, 17, 123,
17, 16, 123,
123, 16, 49,
16, 2, 49,
58, 57, 8,
59, 124, 58,
60, 125, 59,
58, 124, 57,
124, 56, 57,
59, 125, 124,
125, 126, 124,
124, 126, 56,
126, 55, 56,
60, 30, 125,
30, 29, 125,
125, 29, 126,
29, 28, 126,
126, 28, 55,
28, 3, 55,
64, 63, 9,
65, 127, 64,
66, 128, 65,
64, 127, 63,
127, 62, 63,
65, 128, 127,
128, 129, 127,
127, 129, 62,
129, 61, 62,
66, 36, 128,
36, 35, 128,
128, 35, 129,
35, 34, 129,
129, 34, 61,
34, 4, 61,
70, 69, 10,
71, 130, 70,
72, 131, 71,
70, 130, 69,
130, 68, 69,
71, 131, 130,
131, 132, 130,
130, 132, 68,
132, 67, 68,
72, 42, 131,
42, 41, 131,
131, 41, 132,
41, 40, 132,
132, 40, 67,
40, 5, 67,
51, 75, 7,
50, 133, 51,
49, 134, 50,
51, 133, 75,
133, 74, 75,
50, 134, 133,
134, 135, 133,
133, 135, 74,
135, 73, 74,
49, 48, 134,
48, 47, 134,
134, 47, 135,
47, 46, 135,
135, 46, 73,
46, 11, 73,
57, 78, 8,
56, 136, 57,
55, 137, 56,
57, 136, 78,
136, 77, 78,
56, 137, 136,
137, 138, 136,
136, 138, 77,
138, 76, 77,
55, 54, 137,
54, 53, 137,
137, 53, 138,
53, 52, 138,
138, 52, 76,
52, 7, 76,
63, 81, 9,
62, 139, 63,
61, 140, 62,
63, 139, 81,
139, 80, 81,
62, 140, 139,
140, 141, 139,
139, 141, 80,
141, 79, 80,
61, 60, 140,
60, 59, 140,
140, 59, 141,
59, 58, 141,
141, 58, 79,
58, 8, 79,
69, 84, 10,
68, 142, 69,
67, 143, 68,
69, 142, 84,
142, 83, 84,
68, 143, 142,
143, 144, 142,
142, 144, 83,
144, 82, 83,
67, 66, 143,
66, 65, 143,
143, 65, 144,
65, 64, 144,
144, 64, 82,
64, 9, 82,
45, 87, 11,
44, 145, 45,
43, 146, 44,
45, 145, 87,
145, 86, 87,
44, 146, 145,
146, 147, 145,
145, 147, 86,
147, 85, 86,
43, 72, 146,
72, 71, 146,
146, 71, 147,
71, 70, 147,
147, 70, 85,
70, 10, 85,
91, 90, 12,
92, 148, 91,
93, 149, 92,
91, 148, 90,
148, 89, 90,
92, 149, 148,
149, 150, 148,
148, 150, 89,
150, 88, 89,
93, 75, 149,
75, 74, 149,
149, 74, 150,
74, 73, 150,
150, 73, 88,
73, 11, 88,
94, 91, 12,
95, 151, 94,
96, 152, 95,
94, 151, 91,
151, 92, 91,
95, 152, 151,
152, 153, 151,
151, 153, 92,
153, 93, 92,
96, 78, 152,
78, 77, 152,
152, 77, 153,
77, 76, 153,
153, 76, 93,
76, 7, 93,
97, 94, 12,
98, 154, 97,
99, 155, 98,
97, 154, 94,
154, 95, 94,
98, 155, 154,
155, 156, 154,
154, 156, 95,
156, 96, 95,
99, 81, 155,
81, 80, 155,
155, 80, 156,
80, 79, 156,
156, 79, 96,
79, 8, 96,
100, 97, 12,
101, 157, 100,
102, 158, 101,
100, 157, 97,
157, 98, 97,
101, 158, 157,
158, 159, 157,
157, 159, 98,
159, 99, 98,
102, 84, 158,
84, 83, 158,
158, 83, 159,
83, 82, 159,
159, 82, 99,
82, 9, 99,
90, 100, 12,
89, 160, 90,
88, 161, 89,
90, 160, 100,
160, 101, 100,
89, 161, 160,
161, 162, 160,
160, 162, 101,
162, 102, 101,
88, 87, 161,
87, 86, 161,
161, 86, 162,
86, 85, 162,
162, 85, 102,
85, 10, 102 };

    float tempVerts[] = { 0.000000, -1.000000, 0.000000,
0.723607, -0.447220, 0.525725,
-0.276388, -0.447220, 0.850649,
-0.894426, -0.447216, 0.000000,
-0.276388, -0.447220, -0.850649,
0.723607, -0.447220, -0.525725,
0.276388, 0.447220, 0.850649,
-0.723607, 0.447220, 0.525725,
-0.723607, 0.447220, -0.525725,
0.276388, 0.447220, -0.850649,
0.894426, 0.447216, 0.000000,
0.000000, 1.000000, 0.000000,
0.203181, -0.967950, 0.147618,
0.425323, -0.850654, 0.309011,
0.609547, -0.657519, 0.442856,
0.531941, -0.502302, 0.681712,
0.262869, -0.525738, 0.809012,
-0.029639, -0.502302, 0.864184,
-0.232822, -0.657519, 0.716563,
-0.162456, -0.850654, 0.499995,
-0.077607, -0.967950, 0.238853,
0.203181, -0.967950, -0.147618,
0.425323, -0.850654, -0.309011,
0.609547, -0.657519, -0.442856,
0.812729, -0.502301, -0.295238,
0.850648, -0.525736, 0.000000,
0.812729, -0.502301, 0.295238,
-0.483971, -0.502302, 0.716565,
-0.688189, -0.525736, 0.499997,
-0.831051, -0.502299, 0.238853,
-0.753442, -0.657515, 0.000000,
-0.525730, -0.850652, 0.000000,
-0.251147, -0.967949, 0.000000,
-0.831051, -0.502299, -0.238853,
-0.688189, -0.525736, -0.499997,
-0.483971, -0.502302, -0.716565,
-0.232822, -0.657519, -0.716563,
-0.162456, -0.850654, -0.499995,
-0.077607, -0.967950, -0.238853,
-0.029639, -0.502302, -0.864184,
0.262869, -0.525738, -0.809012,
0.531941, -0.502302, -0.681712,
0.860698, -0.251151, -0.442858,
0.951058, 0.000000, -0.309013,
0.956626, 0.251149, -0.147618,
0.956626, 0.251149, 0.147618,
0.951058, -0.000000, 0.309013,
0.860698, -0.251151, 0.442858,
0.687159, -0.251152, 0.681715,
0.587786, 0.000000, 0.809017,
0.436007, 0.251152, 0.864188,
0.155215, 0.251152, 0.955422,
0.000000, -0.000000, 1.000000,
-0.155215, -0.251152, 0.955422,
-0.436007, -0.251152, 0.864188,
-0.587786, 0.000000, 0.809017,
-0.687159, 0.251152, 0.681715,
-0.860698, 0.251151, 0.442858,
-0.951058, -0.000000, 0.309013,
-0.956626, -0.251149, 0.147618,
-0.956626, -0.251149, -0.147618,
-0.951058, 0.000000, -0.309013,
-0.860698, 0.251151, -0.442858,
-0.687159, 0.251152, -0.681715,
-0.587786, -0.000000, -0.809017,
-0.436007, -0.251152, -0.864188,
-0.155215, -0.251152, -0.955422,
0.000000, 0.000000, -1.000000,
0.155215, 0.251152, -0.955422,
0.436007, 0.251152, -0.864188,
0.587786, -0.000000, -0.809017,
0.687159, -0.251152, -0.681715,
0.831051, 0.502299, 0.238853,
0.688189, 0.525736, 0.499997,
0.483971, 0.502302, 0.716565,
0.029639, 0.502302, 0.864184,
-0.262869, 0.525738, 0.809012,
-0.531941, 0.502302, 0.681712,
-0.812729, 0.502301, 0.295238,
-0.850648, 0.525736, 0.000000,
-0.812729, 0.502301, -0.295238,
-0.531941, 0.502302, -0.681712,
-0.262869, 0.525738, -0.809012,
0.029639, 0.502302, -0.864184,
0.483971, 0.502302, -0.716565,
0.688189, 0.525736, -0.499997,
0.831051, 0.502299, -0.238853,
0.753442, 0.657515, 0.000000,
0.525730, 0.850652, 0.000000,
0.251147, 0.967949, 0.000000,
0.077607, 0.967950, 0.238853,
0.162456, 0.850654, 0.499995,
0.232822, 0.657519, 0.716563,
-0.203181, 0.967950, 0.147618,
-0.425323, 0.850654, 0.309011,
-0.609547, 0.657519, 0.442856,
-0.203181, 0.967950, -0.147618,
-0.425323, 0.850654, -0.309011,
-0.609547, 0.657519, -0.442856,
0.077607, 0.967950, -0.238853,
0.162456, 0.850654, -0.499995,
0.232822, 0.657519, -0.716563,
0.052790, -0.723612, 0.688185,
0.138199, -0.894429, 0.425321,
0.361805, -0.723611, 0.587779,
0.670817, -0.723611, -0.162457,
0.670818, -0.723610, 0.162458,
0.447211, -0.894428, 0.000001,
-0.638195, -0.723609, 0.262864,
-0.361801, -0.894428, 0.262864,
-0.447211, -0.723610, 0.525729,
-0.447211, -0.723612, -0.525727,
-0.361801, -0.894429, -0.262863,
-0.638195, -0.723609, -0.262863,
0.361803, -0.723612, -0.587779,
0.138197, -0.894429, -0.425321,
0.052789, -0.723611, -0.688186,
1.000000, 0.000000, 0.000000,
0.947213, -0.276396, 0.162458,
0.947213, -0.276396, -0.162458,
0.309017, 0.000000, 0.951056,
0.138199, -0.276398, 0.951055,
0.447216, -0.276398, 0.850648,
-0.809018, 0.000000, 0.587783,
-0.861803, -0.276396, 0.425324,
-0.670819, -0.276397, 0.688191,
-0.809018, -0.000000, -0.587783,
-0.670819, -0.276397, -0.688191,
-0.861803, -0.276396, -0.425324,
0.309017, -0.000000, -0.951056,
0.447216, -0.276398, -0.850648,
0.138199, -0.276398, -0.951055,
0.670820, 0.276396, 0.688190,
0.809019, -0.000002, 0.587783,
0.861804, 0.276394, 0.425323,
-0.447216, 0.276397, 0.850648,
-0.309017, -0.000001, 0.951056,
-0.138199, 0.276397, 0.951055,
-0.947213, 0.276396, -0.162458,
-1.000000, 0.000001, 0.000000,
-0.947213, 0.276397, 0.162458,
-0.138199, 0.276397, -0.951055,
-0.309016, -0.000000, -0.951057,
-0.447215, 0.276397, -0.850649,
0.861804, 0.276396, -0.425322,
0.809019, 0.000000, -0.587782,
0.670821, 0.276397, -0.688189,
0.361800, 0.894429, 0.262863,
0.447209, 0.723612, 0.525728,
0.638194, 0.723610, 0.262864,
-0.138197, 0.894430, 0.425319,
-0.361804, 0.723612, 0.587778,
-0.052790, 0.723612, 0.688185,
-0.447210, 0.894429, 0.000000,
-0.670817, 0.723611, -0.162457,
-0.670817, 0.723611, 0.162457,
-0.138197, 0.894430, -0.425319,
-0.052790, 0.723612, -0.688185,
-0.361804, 0.723612, -0.587778,
0.361800, 0.894429, -0.262863,
0.638194, 0.723610, -0.262864,
0.447209, 0.723612, -0.525728 };

	//End of code from: http://prideout.net/blog/?p=48
	

	//Iterate through the tempVertes and add or subtract a very small value from each value.
	for(int i = 0; i < sizeof(tempVerts)/sizeof(tempVerts[0]); i++)
	{
		tempVerts[i] += fRand(-.09f, .09f);
	}

	//Now we save the temp variables.
	memcpy(Verts, tempVerts, sizeof(tempVerts));
	memcpy(Faces, tempFaces, sizeof(tempFaces));
}

void Asteroid::draw()
{
	GLfloat ambientMaterial[] = {64/255.0, 21/255.0, 21/255.0, 0 };
	GLfloat difuseMaterial[] = {107/255.0, 34/255.0, 34/255.0, 0  };
	GLfloat shine[] = {.03};

	//Set the color for the lit asteroid.
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv( GL_FRONT, GL_DIFFUSE, difuseMaterial);
	glMaterialfv( GL_FRONT, GL_SHININESS, shine);

	glPushMatrix();
	glTranslatef(x, y, z);
	glPushMatrix();
	glRotatef(angle,angleX,angleY,angleZ);
	glScalef(radius, radius, radius);
	glCallList(name);
	glPopMatrix();
	glPopMatrix();
}