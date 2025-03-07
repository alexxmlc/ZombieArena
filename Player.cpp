#include "Player.h"
#include <iostream>

Player::Player()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	//Create player sprite
	m_Texture.loadFromFile("graphics/player.png");
	m_Sprite.setTexture(m_Texture);

	//Set the origin of the sprite
	m_Sprite.setOrigin(25, 25);
}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize) {
	//Place the player in the middle of the arena
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	//Copy the arena detail to Player's m_Arena
	m_Arena.left = arena.left;
	m_Arena.top = arena.top;
	m_Arena.width = arena.width;
	m_Arena.height = arena.height;

	m_TileSize = tileSize;

	//Store the resolution for future use
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}

void Player::resetPlayerStatus()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}

Time Player::getLastHitTime()
{
	return m_LastHit;
}

bool Player::hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	else
	{
		return false;
	}
}

FloatRect Player::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
	return m_Position;
}

float Player::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return m_Sprite;
}

int Player::getHealth()
{
	return m_Health;
}

void Player::moveLeft()
{
	m_LeftPressed = true;
}

void Player::moveRight()
{
	m_RightPressed = true;
}

void Player::moveDown()
{
	m_DownPressed = true;
}

void Player::moveUp()
{
	m_UpPressed = true;
}

void Player::stopLeft()
{
	m_LeftPressed = false;
}

void Player::stopRight()
{
	m_RightPressed = false;
}

void Player::stopDown()
{
	m_DownPressed = false;
}

void Player::stopUp()
{
	m_UpPressed = false;
}

void Player::update(float elapsedTime, Vector2i mousePosition)
{
	//Update player's position
	if (m_UpPressed)
	{
		m_Position.y -= m_Speed * elapsedTime;
	}

	if (m_DownPressed)
	{
		m_Position.y += m_Speed * elapsedTime;
	}

	if (m_RightPressed)
	{
		m_Position.x += m_Speed * elapsedTime;
	}

	if (m_LeftPressed)
	{
		m_Position.x -= m_Speed * elapsedTime;
	}

	m_Sprite.setPosition(m_Position);


	//Keep theplayer in the arena

	if (m_Position.x > m_Arena.width - m_TileSize)
	{
		m_Position.x = m_Arena.width - m_TileSize;
	}

	if (m_Position.x < m_Arena.left + m_TileSize)
	{
		m_Position.x = m_Arena.left + m_TileSize;
	}

	if (m_Position.y > m_Arena.height - m_TileSize)
	{
		m_Position.y = m_Arena.height - m_TileSize;
	}

	if (m_Position.y < m_Arena.top + m_TileSize)
	{
		m_Position.y = m_Arena.top + m_TileSize;
	}

	//Function to calculate the angle in wich way the player is facing
	//The angle is calculated based on the mouse position and the resolution of the screen
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,mousePosition.x - m_Resolution.x / 2)* 180) / 3.141;
	m_Sprite.setRotation(angle);
}

void Player::upgradeSpeed()
{
	m_Speed += (START_SPEED * .2);
}

void Player::upgradeHealth()
{
	m_Health += (START_HEALTH * .2);
}

void Player::increaseHealthLevel(int amount)
{
	m_Health += amount;
	if (m_Health > m_MaxHealth)
		m_Health = m_MaxHealth;
}