#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	//Player position (the center of the player)
	Vector2f m_Position;

	Sprite m_Sprite;
	Texture m_Texture;

	//Screen resolution
	Vector2f m_Resolution;

	//Size of arena
	IntRect m_Arena;

	int m_TileSize;

	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	int m_Health;
	int m_MaxHealth;

	Time m_LastHit;

	//speed in pixels per second
	float m_Speed;

public:
	Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	//Called every frame
	void resetPlayerStats();

	bool hit(Time timeHit);
	Time getLastHitTime();

	//Where is the player
	FloatRect getPosition();

	//center of the player
	Vector2f getCenter();

	//what angle is the player facing
	float getRotation();

	Sprite getSprite();

	//Player movement
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	//Stop the movement
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	//Called every frame
	void update(float elapsedTime, Vector2i mousePosition);

	//Speed boost for the player
	void upgradeSpeed();

	//Upgrade the health
	void upgradeHealth();

	//Increase max health
	void increaseHealthLevel(int amount);

	//Current health
	int getHealth();

};
	