#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup
{
private:
	//Start values for pickups
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	//Sprite to hold the texture
	Sprite m_Sprite;

	//The arena it exists in
	FloatRect m_Arena;

	//Value of pickup
	int m_Value;

	//Type of pickup 1 = health; 2 = ammo
	int m_Type;

	//Spawning and dissappearing
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDespawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

public:

	//Constructor
	Pickup(int type);

	//Prepare a new pickup
	void setArena(IntRect arena);

	//Spawn the pickup
	void spawn();

	//Get the position
	FloatRect getPosition();

	//Get the sprite
	Sprite getSprite();

	//Pickup update each frame
	void update(float elapsedTime);

	//Check if pickup is spawned
	bool isSpawned();

	//Get the stuff from the pickup
	int gotIt();

	//Upgrade the value of each pickup
	void upgrade();
};