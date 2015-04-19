#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include "Explosion.h"
#include "ShotExplosion.h"
#include "Asteroid.h"
#include "AlienSpaceShip.h"
#include "PlayerSpaceShip.h"
#include "GameObject.h"
#include "Movement.h"
#include "Collisions.h"
#include "GlobalAsteroidVariables.h"
#include "BackGround.h"
#include "Shot.h"
#include "Score.h"
#include "Level.h"
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glut.h>
#include <math.h>
#include <memory>
#include <list>

using namespace std;


Level::Level()
{
	mover = Movement();
	deadCount = 0;
	collider = Collisions();
	alienShip = shared_ptr<AlienSpaceShip>(new AlienSpaceShip());

	lives = 3;
	game_over = false;
	safe = true;
	score = Score();
	count = 0;

	current = 1;
	done = false;
	for(int i = 0; i < 1; i++)
	{
		objects.push_back(shared_ptr<GameObject>(new Asteroid(1)));
	}
	for(int i = 0; i < lives; i++) 
	{
		player_lives.push_back(shared_ptr<GameObject>(new Life(i)));
	}
}

Level::Level(int numberOfAsteroids, shared_ptr<PlayerSpaceShip> player, int lives_to_start, long score_to_start)
{

	lives = lives_to_start;
	game_over = false;
	safe = true;
	score = Score(score_to_start);

	//Cutoff at the maximum number of asteroids if needed.
	if(numberOfAsteroids > MAX_ASTEROIDS)
		numberOfAsteroids = MAX_ASTEROIDS;

	playerShip = player;
	collider = Collisions();
	mover = Movement();
	deadCount = 0;
	count = 0;

	done = false;
	current = numberOfAsteroids;
	alienShip = shared_ptr<AlienSpaceShip>(new AlienSpaceShip(false));

	//alienShip->alive = true;

	for(int i = 0; i < numberOfAsteroids; i++)
	{
		objects.push_back(shared_ptr<GameObject>(new Asteroid(1)));
	}
	for(int i = 0; i < lives; i++) 
	{
		player_lives.push_back(shared_ptr<GameObject>(new Life(i)));
	}

}

Level::~Level()
{
	//This is not needed since we are using shared_ptr.

	//for each(unique_ptr<GameObject> toDelete in objects)
	//{
	//	delete toDelete;
	//}
}

void Level::draw()
{
	stars->draw();
	playerShip->draw();
	alienShip->draw();

	for each(shared_ptr<GameObject> toDraw in objects)
	{
		toDraw->draw();
	}

	for each(shared_ptr<GameObject> toDraw in pshots)
	{
		toDraw->draw();
	}
	
	for each(shared_ptr<GameObject> toDraw in ashots)
	{
		toDraw->draw();
	}

	for each(shared_ptr<Explosion> toDraw in explosions)
	{
		toDraw->draw();
	}

	for each(shared_ptr<GameObject> toDraw in player_lives)
	{
		toDraw->draw();
	}

	score.draw();

	if(game_over)
	{
		string value = "Game Over";

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
		glTranslatef(-size/152.38, 0.0, 2);
		glScalef(2/152.38, 2/152.38, 2/152.38);
		for (int i = 0; i < length; i++) 
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, value[i]);
		}

		glPopMatrix();

		value = "Right click to start a new game or quit.";

		//Get the length of the string.
		length = value.size();
		size = 0;

		//Use Glut to display the string.

		for (int i = 0; i < length; i++) 
		{
			size += glutStrokeWidth(GLUT_STROKE_ROMAN, value[i]);
		}

		glPushMatrix();
		glTranslatef(-size/304.76, -10, 2);
		glScalef(1/152.38, 1/152.38, 1/152.38);
		for (int i = 0; i < length; i++) 
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, value[i]);
		}

		glPopMatrix();
		glMaterialfv( GL_FRONT, GL_EMISSION, none);
	}
}

void Level::update()
{
	list<shared_ptr<Shot>> shotsTemp = pshots;
	list<shared_ptr<GameObject>> objectsTemp;

	if(objects.size() > 0 || explosions.size() > 0 || alienShip->alive) 
	{

		objectsTemp = objects;

		//If the player ship is rotating update the rotation.
		mover.rotate(playerShip);
		mover.move(playerShip);

		mover.move(alienShip);

		for each(shared_ptr<GameObject> toUpdate in objects)
		{
			mover.rotate(toUpdate);
			mover.move(toUpdate);
		}
		for each(shared_ptr<GameObject> toUpdate in player_lives)
		{
			mover.rotate(toUpdate);
		}

		
		//Handle the dead player ship.
		if(!playerShip->alive && !game_over)	
		{
			if(deadCount > FRAMES_ON_DEATH && safe)
			{
				deadCount = 0;

				//This is where the UI will update the player lives or set a game over condition.


				//Place the player at the center of the screen again.
				playerShip->x = 0.0f;
				playerShip->y = 0.0f;
				playerShip->alive = true;
			}
			else
			{
				deadCount++;
			}
		}

		safe = true;

		//Perform collision checks.
		for each(shared_ptr<GameObject> object in objectsTemp)
		{
		
			if(collider.dist(0.0f, object->x, 0.0f, object->y) < SAFE_ZONE)
				safe = false;

			shotsTemp = pshots;

			//Try to cast the game object to asteroid.
			shared_ptr<Asteroid> asteroid = dynamic_pointer_cast<Asteroid>(object);
			//See that the asteroid is casted.
			if(asteroid.use_count() > 0)
			{
				//We are going to check for collisions between all objects.
				//Start by checking all asteroids and shots.
				for each(shared_ptr<Shot> deadly in shotsTemp)
				{
					//See if the shot collides.
					if(collider.shotAndAsteroid(asteroid, deadly))
					{
						//Create explosion here.
						explosions.push_back(shared_ptr<ShotExplosion>(new ShotExplosion(asteroid->x, asteroid->y, deadly->velocityX/2, deadly->velocityY/2, 107/255.0, 34/255.0, 34/255.0)));

						//Split the asteroid.
						list<shared_ptr<GameObject>> toAdd = asteroid->split(deadly->velocityX, deadly->velocityY);

						//Add the returned asteroids to the list of asteroids.
						for each(shared_ptr<GameObject> add in toAdd)
							objects.push_front(add);

						//Score the asteroid points.
						if(score.addScore(object->points))
							player_lives.push_back(shared_ptr<GameObject>(new Life(player_lives.size())));

						//Remove the asteroid from the list of objects.
						objects.remove(object);

						//Remove the deadly shot.
						pshots.remove(deadly);

					}

				}

				shotsTemp = ashots;

				for each(shared_ptr<Shot> deadly in shotsTemp)
				{
					//See if the shot collides.
					if(collider.shotAndAsteroid(asteroid, deadly))
					{
						//Create explosion here.
						explosions.push_back(shared_ptr<ShotExplosion>(new ShotExplosion(asteroid->x, asteroid->y, deadly->velocityX, deadly->velocityY, 107/255.0, 34/255.0, 34/255.0)));

						//Split the asteroid.
						list<shared_ptr<GameObject>> toAdd = asteroid->split(deadly->velocityX, deadly->velocityY);

						//Add the returned asteroids to the list of asteroids.
						for each(shared_ptr<GameObject> add in toAdd)
							objects.push_front(add);

						//Remove the asteroid from the list of objects.
						objects.remove(object);

						//Remove the deadly shot.
						ashots.remove(deadly);

					}

				}

				//Get the distance to the player ship.
				float dist = collider.dist(playerShip->x, asteroid->x, playerShip->y, asteroid->y);

				//If the asteroid is near the ship then perform collision checks.
				if(dist < COLLISION_RADIUS) 
				{

					//Check to see if the player ship is hit.
					if(playerShip->alive && collider.shipAndAsteroid(asteroid, playerShip))
					{
						//Destroy the ship and split the asteroid.
						playerShip->alive = false;
						removeLife();
	
						//Create explosion here.
						explosions.push_back(shared_ptr<Explosion>(new Explosion(playerShip->x, playerShip->y, asteroid->velocityX/2, asteroid->velocityY/2, 63/255.0f, 69/255.0f, 101/255.0f)));
						explosions.push_back(shared_ptr<Explosion>(new Explosion(asteroid->x, asteroid->y,  playerShip->velocityX/2, playerShip->velocityY/2, 107/255.0, 34/255.0, 34/255.0)));

						//Get the direction from ship to asteroid.
						float dx = playerShip->x - asteroid->x;
						float dy = playerShip->y - asteroid->y;

						//Split the asteroid.
						list<shared_ptr<GameObject>> toAdd = asteroid->split(dx, dy);

						//Add the returned asteroids to the list of asteroids.
						for each(shared_ptr<GameObject> add in toAdd)
							objects.push_front(add);

						//Remove the asteroid from the list of objects.
						objects.remove(asteroid);

						//Reset the player ship.
						playerShip->acceleration = 0;
						playerShip->velocityX = 0;
						playerShip->velocityY = 0;
						playerShip->angle = 0;
					}
				}

				//Get the distance to the alien ship.
				dist = collider.dist(alienShip->x, asteroid->x, alienShip->y, asteroid->y);

				//If the asteroid is near the ship then perform collision checks.
				if(dist < COLLISION_RADIUS) 
				{
					//Check to see if the alien ship is hit.
					if(alienShip->alive && collider.shipAndAsteroid(asteroid, alienShip))
					{
						//Destroy the ship and split the asteroid.
						alienShip->alive = false;
						alienShip->dead = true;
	
						//Create explosion here.
						explosions.push_back(shared_ptr<Explosion>(new Explosion(alienShip->x, alienShip->y, asteroid->velocityX/2, asteroid->velocityY/2, 0.329412, 0.329412, 0.329412)));
						explosions.push_back(shared_ptr<Explosion>(new Explosion(asteroid->x, asteroid->y,  alienShip->velocityX/2, alienShip->velocityY/2, 107/255.0, 34/255.0, 34/255.0)));


						//Get the direction from ship to asteroid.
						float dx = alienShip->x - asteroid->x;
						float dy = alienShip->y - asteroid->y;

						//Split the asteroid.
						list<shared_ptr<GameObject>> toAdd = asteroid->split(dx, dy);

						//Add the returned asteroids to the list of asteroids.
						for each(shared_ptr<GameObject> add in toAdd)
							objects.push_front(add);

						//Remove the asteroid from the list of objects.
						objects.remove(asteroid);

						//Reset the alien ship.
						alienShip->acceleration = 0;
						alienShip->velocityX = 0;
						alienShip->velocityY = 0;
						alienShip->angle = 0;
					}
				}


			}

		}

		//Check for collisions between the alien ship and player shots.
		if(alienShip->alive)
		{
			shotsTemp = pshots;

			for each(shared_ptr<Shot> deadly in shotsTemp)
			{
				if(collider.dist(deadly->x, alienShip->x, deadly->y, alienShip->y) < COLLISION_RADIUS)
				{
					//If the shot hits the ship 
					if(collider.shotAndShip(deadly, alienShip))
					{
						//Destroy the ship.
						alienShip->alive = false;
						alienShip->dead = true;
						if(score.addScore(alienShip->points))
							player_lives.push_back(shared_ptr<GameObject>(new Life(player_lives.size())));

						explosions.push_back(shared_ptr<ShotExplosion>(new ShotExplosion(alienShip->x, alienShip->y,  deadly->velocityX, deadly->velocityY, 0.329412, 0.329412, 0.329412)));

						//Remove the shot.
						pshots.remove(deadly);

						//Reset the alien ship.
						alienShip->acceleration = 0;
						alienShip->velocityX = 0;
						alienShip->velocityY = 0;
						alienShip->angle = 0;
					}
				}
			}
		}


		//Check for collisions between the player ship and alien shots.
		if(playerShip->alive)
		{
			shotsTemp = ashots;

			for each(shared_ptr<Shot> deadly in shotsTemp)
			{
				if(collider.dist(deadly->x, playerShip->x, deadly->y, playerShip->y) < COLLISION_RADIUS)
				{
					//If the shot hits the ship 
					if(collider.shotAndShip(deadly, playerShip))
					{
						//Destroy the ship.
						playerShip->alive = false;
						pshots.remove(deadly);
						removeLife();

						//Create explosion here.
						explosions.push_back(shared_ptr<ShotExplosion>(new ShotExplosion(playerShip->x, playerShip->y, deadly->velocityX, deadly->velocityY, 63/255.0f, 69/255.0f, 101/255.0f)));
						

						//Reset the player ship.
						playerShip->acceleration = 0;
						playerShip->velocityX = 0;
						playerShip->velocityY = 0;
						playerShip->angle = 0;
					}
				}
			}
		}

		//Check for player and alien collision.
		if(playerShip->alive && alienShip->alive && collider.dist(playerShip->x, alienShip->x, playerShip->y, alienShip->y) < COLLISION_RADIUS)
		{
			if(collider.shipAndShip(playerShip, alienShip))
			{
						//Destroy the player ship.
						playerShip->alive = false;
						removeLife();

						//Create explosion here.
						explosions.push_back(shared_ptr<Explosion>(new Explosion(playerShip->x, playerShip->y, alienShip->velocityX/2, alienShip->velocityY/2, 63/255.0f, 69/255.0f, 101/255.0f)));
						explosions.push_back(shared_ptr<Explosion>(new Explosion(alienShip->x, alienShip->y, alienShip->velocityX/2, playerShip->velocityY/2, 0.329412, 0.329412, 0.329412)));
						

						//Reset the player ship.
						playerShip->acceleration = 0;
						playerShip->velocityX = 0;
						playerShip->velocityY = 0;
						playerShip->angle = 0;

						//Destroy the alien ship.
						alienShip->alive = false;
						alienShip->dead = true;

						//Reset the alien ship.
						alienShip->acceleration = 0;
						alienShip->velocityX = 0;
						alienShip->velocityY = 0;
						alienShip->angle = 0;
			}
		}

		shotsTemp = pshots;

		for each(shared_ptr<Shot> toUpdate in shotsTemp)
		{
			//We are going to check the shots vector for a end of lifespan shot.
			//If we find it we pop the shot. This should work because the older ones 
			//are in the back of the vector.
			if(toUpdate->lifeCount > MAX_SHOT_LIFE)
				pshots.pop_front();
			else
			{
				//Move the shot.
				mover.move(toUpdate);

				//Increase the lifespan count of the shot.
				toUpdate->lifeCount++;
			}
		}

		list<shared_ptr<Explosion>> explosionsTemp = explosions;

		for each(shared_ptr<Explosion> toUpdate in explosionsTemp)
		{
			//We are going to check the shots vector for a end of lifespan shot.
			//If we find it we pop the shot. This should work because the older ones 
			//are in the back of the vector.
			if(toUpdate->life > MAX_EXPLOSION_LIFE)
				explosions.pop_front();
			else
				toUpdate->life++;

		}

		shotsTemp = ashots;

		for each(shared_ptr<Shot> toUpdate in shotsTemp)
		{
			//We are going to check the shots vector for a end of lifespan shot.
			//If we find it we pop the shot. This should work because the older ones 
			//are in the back of the vector.
			if(toUpdate->lifeCount > MAX_SHOT_LIFE)
				ashots.pop_front();
			else
			{
				//Move the shot.
				mover.move(toUpdate);

				//Increase the lifespan count of the shot.
				toUpdate->lifeCount++;
			}
		}
		if(alienShip->alive && ashots.size() == 0)
		{
			makeAlienShot();
		}

		if(alienShip->dead && !alienShip->is_small)
			alienShip = shared_ptr<AlienSpaceShip>(new AlienSpaceShip(true));
		////Alien ship.
		alienShip->update(count++, score.getScore(), objects.size());


	}
	else
	{
		done = true;
	}

}

void Level::makeShot()
{
	if(playerShip->alive) 
	{
		//Get the angle of rotation for the object.
		double angle = playerShip->angle*PI/180;

		if(playerShip->angleZ < 0)
			angle = -angle;

		float xVel = playerShip->shotVelocity*sin(angle) + playerShip->velocityX;//Adding the playerShip velocity removes the velocity of the player ship.
		float yVel = -playerShip->shotVelocity*cos(angle) + playerShip->velocityY;//Adding the playerShip velocity removes the velocity of the player ship.
	
		if(pshots.size() < MAX_SHOTS)
			pshots.push_back(shared_ptr<Shot>(new Shot(playerShip->x, playerShip->y, xVel, yVel, playerShip->angle)));
		//Note that we are inserting in the back to keep the older shots in the front.
	}
}

void Level::removeLife()
{
	player_lives.pop_back();
	if(player_lives.size() == 0)
		game_over = true;
}

void Level::makeAlienShot()
{
	int player_s = 0;
	int player_f = alienShip->p_p - 1;
	int asteroid_s = player_f + 1;
	int asteroid_f = player_f + alienShip->p_a;
	float dirX = 0;
	float dirY = 0;
	float xVel = 0;
	float yVel = 0;
	float angle = 0;

	int random = rand() % 101;

	if(random >= player_s && random <= player_f)
	{
		dirX = playerShip->x - (alienShip->x + alienShip->velocityX);
		dirY = playerShip->y - (alienShip->y + alienShip->velocityY);

		shared_ptr<PlayerSpaceShip> toShoot = playerShip;

		//Perform some targeting calculations.
		for(int i = 0; i < MAX_SHOT_LIFE; i++)
		{
			float x = toShoot->x + toShoot->velocityX*i;
			float y = toShoot->y + toShoot->velocityY*i;

			float x_a = alienShip->x + alienShip->velocityX*i;
			float y_a = alienShip->y + alienShip->velocityY*i;

			float distance = collider.dist(x, x_a, y, y_a);
			
			if(abs(distance - alienShip->shotVelocity*i) <= SHIP_BACK)
			{
				dirX = x - x_a;
				dirY = y - y_a;
				break;
			}
			
		}

		double mag = sqrt(dirX*dirX + dirY*dirY);

		xVel = alienShip->shotVelocity*(dirX/mag) + alienShip->velocityX;
		yVel = -alienShip->shotVelocity*(dirY/mag) + alienShip->velocityY;

		ashots.push_back(shared_ptr<Shot>(new Shot(alienShip->x, alienShip->y, xVel, yVel, 0)));
	}
	else if(random >= asteroid_s && random <= asteroid_f && objects.size() > 1)
	{
		shared_ptr<GameObject> toShoot;

		float close = FLT_MAX;
		float current = 0.0f;

		for each(shared_ptr<GameObject> asteroid in objects)
		{
			current = collider.dist(asteroid->x, alienShip->x, asteroid->y, alienShip->y);

			if(current < close)
			{
				toShoot = asteroid;
				close = current;
			}
		}

		dirX = (toShoot->x + toShoot->velocityX) - (alienShip->x + alienShip->velocityX);
		dirY = (toShoot->y + toShoot->velocityY) - (alienShip->y + alienShip->velocityY);

		//Perform some targeting calculations.
		for(int i = 1; i <= MAX_SHOT_LIFE; i++)
		{
			float x = toShoot->x + toShoot->velocityX*i;
			float y = toShoot->y + toShoot->velocityY*i;

			float x_a = alienShip->x + alienShip->velocityX*i;
			float y_a = alienShip->y + alienShip->velocityY*i;

			float distance = collider.dist(x, x_a, y, y_a);
			
			if(abs(distance - alienShip->shotVelocity*i) <= .5)
			{
				dirX = x - x_a;
				dirY = y - y_a;
				break;
			}
			
		}

		double mag = sqrt(dirX*dirX + dirY*dirY);

		xVel = alienShip->shotVelocity*(dirX/mag) + alienShip->velocityX;
		yVel = -alienShip->shotVelocity*(dirY/mag) + alienShip->velocityY;

		ashots.push_back(shared_ptr<Shot>(new Shot(alienShip->x, alienShip->y, xVel, yVel, 0)));
	}
	else
	{
		dirX = fRand(-15.0, 15.0) - alienShip->x;
		dirY = fRand(-15.0, 15.0) - alienShip->y;

		angle = getAngle(dirX, dirY);

		xVel = alienShip->shotVelocity*sin(angle) + alienShip->velocityX;
		yVel = -alienShip->shotVelocity*cos(angle) + alienShip->velocityY;

		ashots.push_back(shared_ptr<Shot>(new Shot(alienShip->x, alienShip->y, xVel, yVel, angle)));
	}
}


float Level::getAngle(float x, float y)
{
	if(x == 0)
		return 0;
	return (float)atan2((double)y,(double)x) - PI;
}