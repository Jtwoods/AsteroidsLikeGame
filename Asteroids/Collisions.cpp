#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include "Collisions.h"
#include "GlobalAsteroidVariables.h"
#include <math.h>
#include <iostream>
#include <memory>

#include "Asteroid.h"

using namespace std;

Collisions::Collisions() 
{
}

bool Collisions::shotAndAsteroid(shared_ptr<Asteroid> asteroid, shared_ptr<Shot> shot) 
{
	//Get the position and radius of the asteroid.
	shared_ptr<GameObject> obj = dynamic_pointer_cast<GameObject>(asteroid);
	float astx = obj->x;
	float asty = obj->y;
	float astRad = asteroid->radius + SHOT_RADIUS;

	//Get the postion of the shot.
	float shotx = shot->x;
	float shoty = shot->y;

	//Get the distance from the center of the asteroid to the shot.

	float d = dist(shotx, astx, shoty, asty);

	if(d < astRad)
		return true;

	return false;
}

bool Collisions::shipAndAsteroid(shared_ptr<Asteroid> asteroid, shared_ptr<PlayerSpaceShip> playerShip)
{

	//Get the angle of rotation for the object.
	double angle = playerShip->angle*PI/HALF_CIRCLE;

	if(playerShip->angleZ < 0)
		angle = -angle;

	//Build the needed variables.
	float frontX = playerShip->x + (float)sin(angle);
	float frontY = playerShip->y - (float)cos(angle);

	angle += PI_OVER_TWO;

	float rightX = playerShip->x + (float)sin(angle)*SHIP_SIDE;
	float rightY = playerShip->y - (float)cos(angle)*SHIP_SIDE;

	angle -= PI_OVER_TWO + PI_OVER_TWO;

	float leftX = playerShip->x + (float)sin(angle)*SHIP_SIDE;
	float leftY = playerShip->y - (float)cos(angle)*SHIP_SIDE;

	angle += PI_OVER_TWO + PI;

	float backX = playerShip->x + (float)sin(angle)*SHIP_BACK;
	float backY = playerShip->y - (float)cos(angle)*SHIP_BACK;

	//Are any of the points inside the asteroid?
	if(dist(frontX, asteroid->x, frontY, asteroid->y) < asteroid->radius)
		return true;
	if(dist(backX, asteroid->x, backY, asteroid->y) < asteroid->radius)
		return true;
	if(dist(leftX, asteroid->x, leftY, asteroid->y) < asteroid->radius)
		return true;
	if(dist(rightX, asteroid->x, rightY, asteroid->y) < asteroid->radius)
		return true;

	//Do any of the lines intersect?
	if(intersect(frontX, rightX, frontY, rightY, asteroid->x, asteroid->y, asteroid->radius))
		return true;
	else if(intersect(backX, rightX, backY, rightY, asteroid->x, asteroid->y, asteroid->radius))
		return true;
	else if(intersect(backX, leftX, backY, leftY, asteroid->x, asteroid->y, asteroid->radius))
		return true;
	else if(intersect(frontX, leftX, frontY, leftY, asteroid->x, asteroid->y, asteroid->radius))
		return true;

	return false;
}

bool Collisions::shipAndAsteroid(shared_ptr<Asteroid> asteroid, shared_ptr<AlienSpaceShip> alienShip)
{

	//Build the needed variables.
	float frontX = alienShip->x;
	float frontY = alienShip->y + alienShip->height;

	float rightX = alienShip->x +  alienShip->width;
	float rightY = alienShip->y;

	float leftX = alienShip->x - alienShip->width;
	float leftY = alienShip->y;

	float backX = alienShip->x;
	float backY = alienShip->y - alienShip->height;

	//Are any of the points inside the asteroid?
	if(dist(frontX, asteroid->x, frontY, asteroid->y) < asteroid->radius)
		return true;
	if(dist(backX, asteroid->x, backY, asteroid->y) < asteroid->radius)
		return true;
	if(dist(leftX, asteroid->x, leftY, asteroid->y) < asteroid->radius)
		return true;
	if(dist(rightX, asteroid->x, rightY, asteroid->y) < asteroid->radius)
		return true;

	//Do any of the lines intersect?
	if(intersect(frontX, rightX, frontY, rightY, asteroid->x, asteroid->y, asteroid->radius))
		return true;
	else if(intersect(backX, rightX, backY, rightY, asteroid->x, asteroid->y, asteroid->radius))
		return true;
	else if(intersect(backX, leftX, backY, leftY, asteroid->x, asteroid->y, asteroid->radius))
		return true;
	else if(intersect(frontX, leftX, frontY, leftY, asteroid->x, asteroid->y, asteroid->radius))
		return true;

	return false;
}

float Collisions::dist(float xOne, float xTwo, float yOne, float yTwo)
{
	float dx = xOne - xTwo;
	float dy = yOne - yTwo;

	return sqrt(dx*dx + dy*dy);
}

int Collisions::sgn(float toCheck)
{
	if(toCheck < 0)
		return -1;
	return 1;
}

float Collisions::smallIntersectX(float dx, float dy, float d, float D, float radius)
{
	if(d == 0)
		return 0;

	return (D*dy - sgn(dy)*dx*sqrt(radius*radius*d - D*D))/d;
}

float Collisions::smallIntersectY(float dx, float dy, float d, float D, float radius)
{
	if(d == 0)
		return 0;

	return (-D*dx - abs(dy)*sqrt(radius*radius*d - D*D))/d;
}

float Collisions::largeIntersectX(float dx, float dy, float d, float D, float radius)
{
	if(d == 0)
		return 0;

	return (D*dy + sgn(dy)*dx*sqrt(radius*radius*d - D*D))/d;
}

float Collisions::largeIntersectY(float dx, float dy, float d, float D, float radius)
{
	if(d == 0)
		return 0;

	return (-D*dx + abs(dy)*sqrt(radius*radius*d - D*D))/d;
}

bool Collisions::intersect(float xOne, float xTwo, float yOne, float yTwo, float xCenter, float yCenter, float radius)
{
	float delta = 0;

	float xLarge = 0;
	float xSmall = 0;
	float yLarge = 0;
	float ySmall = 0;

	float dx = xOne - xTwo;
	float dy = yOne - yTwo;

	float d = dx*dx + dy*dy;
	float D = (xOne - xCenter)*(yTwo - yCenter) - (xTwo - xCenter)*(yOne - yCenter);

	delta = radius*radius*d - D*D;

	if(delta >= 0)
	{
		float xfirst = smallIntersectX( dx, dy, d, D, radius) + xCenter;
		float yfirst = smallIntersectY( dx, dy, d, D, radius) + yCenter;
		float xsecond = largeIntersectX( dx, dy, d, D, radius) + xCenter;
		float ysecond = largeIntersectY( dx, dy, d, D, radius) + yCenter;


		if(xOne > xTwo)
		{
			xLarge = xOne;
			xSmall = xTwo;
		}
		else 
		{
			xSmall = xOne;
			xLarge = xTwo;
		}
		if(yOne > yTwo)
		{
			ySmall = yTwo;
			yLarge = yOne;
		}
		else
		{
			yLarge = yTwo;
			ySmall = yOne;
		}

		if((xfirst > xSmall && xfirst < xLarge && yfirst > ySmall && yfirst < yLarge) || (xsecond > xSmall && xsecond < xLarge && ysecond > ySmall && ysecond < yLarge))
			return true;
	}

	return false;
}

bool Collisions::lineLineIntersection(float xOne, float yOne, float xTwo, float yTwo, float xThree, float yThree, float xFour, float yFour)
{
	//First we get the components of the parametric equation.
	float xTwoOne = xTwo - xOne;
	float yTwoOne = yTwo - yOne;
	float xThreeOne = xThree - xOne;
	float yThreeOne = yThree - yOne;
	float xFourThree = xFour - xThree;
	float yFourThree = yFour - yThree;

	float denom = xFourThree*yTwoOne - xTwoOne*yFourThree;

	if(denom == 0)
		return false;

	float s = (xFourThree*yThreeOne - xThreeOne*yFourThree)/denom;
	float t = (xTwoOne*yThreeOne - xThreeOne*yTwoOne)/denom;

	if(s > 0 && s < 1 && t > 0 && t < 1)
		return true;

	return false;
}

bool Collisions::shotAndShip(shared_ptr<Shot> shot, shared_ptr<AlienSpaceShip> alienShip)
{
	//Build an array of points for the alien ship.
	float coords[8];
	float castY = 0;
	int count = 0;

	coords[0] = alienShip->x;
	coords[1] = alienShip->y + alienShip->height;

	coords[2] = alienShip->x +  alienShip->width;
	coords[3] = alienShip->y;

	coords[4] = alienShip->x - alienShip->width;
	coords[5] = alienShip->y;

	coords[6] = alienShip->x;
	coords[7] = alienShip->y - alienShip->height;

	//Cast the ray from the shot to the edge of the screen.
	if(shot->y > 0)
		castY = -HEIGHT;
	else
		castY = HEIGHT;

	//Now iterate through the array and count the number of intersections.
	for(int i = 3; i < 8; i += 2)
	{
		if(lineLineIntersection(coords[i - 3], coords[i - 2], coords[i - 1], coords[i], shot->x, shot->y, shot->x, castY))
			count++;
	}

	if(count != 0 && (count == 1 || count % 3))
		return true;

	return false;
}

bool Collisions::shotAndShip(shared_ptr<Shot> shot, shared_ptr<PlayerSpaceShip> playerShip)
{
	//Build an array of points for the player ship.
	float coords[8];
	float castY = 0;
	int count = 0;

	//Get the angle of rotation for the object.
	double angle = playerShip->angle*PI/HALF_CIRCLE;

	if(playerShip->angleZ < 0)
		angle = -angle;

	//Build the needed variables.
	coords[0] = playerShip->x + (float)sin(angle);
	coords[1] = playerShip->y - (float)cos(angle);

	angle += PI_OVER_TWO;

	coords[2] = playerShip->x + (float)sin(angle)*SHIP_SIDE;
	coords[3] = playerShip->y - (float)cos(angle)*SHIP_SIDE;

	angle -= PI_OVER_TWO + PI_OVER_TWO;

	coords[4] = playerShip->x + (float)sin(angle)*SHIP_SIDE;
	coords[5] = playerShip->y - (float)cos(angle)*SHIP_SIDE;

	angle += PI_OVER_TWO + PI;

	coords[6] = playerShip->x + (float)sin(angle)*SHIP_BACK;
	coords[7] = playerShip->y - (float)cos(angle)*SHIP_BACK;

	//Cast the ray from the shot to the edge of the screen.
	if(shot->y > 0)
		castY = -HEIGHT;
	else
		castY = HEIGHT;

	//Now iterate through the array and count the number of intersections.
	for(int i = 3; i < 8; i += 2)
	{
		if(lineLineIntersection(coords[i - 3], coords[i - 2], coords[i - 1], coords[i], shot->x, shot->y, shot->x, castY))
			count++;
	}

	if(count != 0 && (count == 1 || count % 3))
		return true;

	return false;
}

bool Collisions::shipAndShip(shared_ptr<PlayerSpaceShip> playerShip, shared_ptr<AlienSpaceShip> alienShip)
{
	//Build an array of points for the player ship.
	float pCoords[8];
	float castY = 0;
	int count = 0;

	//Get the angle of rotation for the object.
	double angle = playerShip->angle*PI/HALF_CIRCLE;

	if(playerShip->angleZ < 0)
		angle = -angle;

	//Build the needed variables.
	pCoords[0] = playerShip->x + (float)sin(angle);
	pCoords[1] = playerShip->y - (float)cos(angle);

	angle += PI_OVER_TWO;

	pCoords[2] = playerShip->x + (float)sin(angle)*SHIP_SIDE;
	pCoords[3] = playerShip->y - (float)cos(angle)*SHIP_SIDE;

	angle -= PI_OVER_TWO + PI_OVER_TWO;

	pCoords[4] = playerShip->x + (float)sin(angle)*SHIP_SIDE;
	pCoords[5] = playerShip->y - (float)cos(angle)*SHIP_SIDE;

	angle += PI_OVER_TWO + PI;

	pCoords[6] = playerShip->x + (float)sin(angle)*SHIP_BACK;
	pCoords[7] = playerShip->y - (float)cos(angle)*SHIP_BACK;

	//Build an array of points for the alien ship.
	float aCoords[8];

	aCoords[0] = alienShip->x;
	aCoords[1] = alienShip->y + alienShip->height;

	aCoords[2] = alienShip->x +  alienShip->width;
	aCoords[3] = alienShip->y;

	aCoords[4] = alienShip->x - alienShip->width;
	aCoords[5] = alienShip->y;

	aCoords[6] = alienShip->x;
	aCoords[7] = alienShip->y - alienShip->height;


	//Check each line in the alien ship for intersection with the lines in the player ship.
	for(int p = 3; p < 8; p += 2)
	{
		for(int a = 3; a < 8; a += 2)
		{
			if(lineLineIntersection(aCoords[a - 3], aCoords[a - 2], aCoords[a - 1], aCoords[a], pCoords[p - 3], pCoords[p - 2], pCoords[p - 1], pCoords[p]))
				return true;
		}
	}

	//Check each point in the player ship to see if it is inside the alien ship.
	for(int p = 1; p < 8; p += 2)
	{
		count = 0;

		//Cast the ray from the current point to the edge of the screen.
		if(pCoords[p] > 0)
			castY = -HEIGHT;
		else
			castY = HEIGHT;

		for(int a = 3; a < 8; a += 2)
		{
			if(lineLineIntersection(aCoords[a - 3], aCoords[a - 2], aCoords[a - 1], aCoords[a], pCoords[p - 1], pCoords[p], pCoords[p - 1], castY))
				count++;
		}

		//If the current count is odd the point is inside the alien ship.
		if(count != 0 && (count == 1 || count % 3))
			return true;
	}

	return false;
}