#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet
{
private:
	
	//Bullet location
	Vector2f m_Position;

	//Bullet shape
	RectangleShape m_BulletShape;

	//Is the bullet currenlt in the air
	bool m_InFlight = false;

	//Bullet Speed
	float m_BulletSpeed = 1000;

	//What fraction of 1 pixel does the bullet travel each frame on X and Y axis
	float m_BulletDistanceX;
	float m_BulletDistanceY;

	//Boundaries so the bullet stopps flying
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

public:

	//Constructor
	Bullet();

	//Stop the bullet
	void stop();

	//Check if bullet is flying
	bool isInFlight();

	//Launch a new bullet
	void shoot(float startX, float startY, float targetX, float targetY	);

	//Bullet position
	FloatRect getPosition();

	//Get the bullet shape for drawing 
	RectangleShape getShape();

	//Update the bullet each frame
	void update(float elapsedTime);
};
