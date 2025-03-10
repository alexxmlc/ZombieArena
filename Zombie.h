#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie
{
private:

	//Zombie speed for each type
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	//Zombie health for each type
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	//Sloghtly vary each zombie speed
	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

	//Zombie position
	Vector2f m_Position;

	//Sprite for the zombie to hold a texture and have boundaries
	Sprite m_Sprite;

	//Zombie speed
	float m_Speed;

	//Zombie health
	float m_Health;

	//Dead status
	bool m_Alive;

public:

	//Handle bullet hits zombie
	bool hit();

	//Dead status
	bool Alive();

	//Spawn a new zombie
	void Spawn(float startX, float startY, int type, int seed);

};