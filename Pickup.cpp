#include "Pickup.h"
#include "TextureHolder.h"

Pickup::Pickup(int type)
{
	m_Type = type;

	//Based on the type of pickup give it the corresponding texture and the starting values
	if (m_Type == 1)
	{
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/health_pickup.png"));
		m_Value = HEALTH_START_VALUE;
	}
	else
	{
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/ammo_pickup.png"));
		m_Value = AMMO_START_VALUE;
	}

	m_Sprite.setOrigin(25, 25);

	m_SecondsToLive = START_SECONDS_TO_LIVE;
	m_SecondsToWait = START_WAIT_TIME;
}

void Pickup::setArena(IntRect arena)
{
	//Add and subtract 50 because the border is 50 pixels wide so the pickup doesn't spawn on the border
	m_Arena.left = arena.left + 50;
	m_Arena.height = arena.height - 50;
	m_Arena.top = arena.top + 50;
	m_Arena.width = arena.width - 50;

	spawn();
}

void Pickup::spawn()
{
	//Spawn at random location of coordinates (x, y)
	srand((int)time(0) / m_Type);
	int x = (rand() % (int)m_Arena.width);

	srand((int)time(0) * m_Type);
	int y = (rand() % (int)m_Arena.height);

	m_SecondsSinceSpawn = 0;
	m_Spawned = true;

	m_Sprite.setPosition(x, y);
}

FloatRect Pickup::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Pickup::getSprite()
{
	return m_Sprite;
}

bool Pickup::isSpawned()
{
	return m_Spawned;
}

int Pickup::gotIt()
{
	m_Spawned = false;
	m_SecondsSinceDespawn = 0;
	return m_Value;
}

void Pickup::update(float elapsedTime)
{
	if (m_Spawned)
	{
		m_SecondsSinceSpawn += elapsedTime;
	}
	else
	{
		m_SecondsSinceDespawn += elapsedTime;
	}

	//Despawn the pickup
	if (m_SecondsSinceSpawn > m_SecondsToLive && m_Spawned)
	{
		m_Spawned = false;
		m_SecondsSinceDespawn = 0;
	}

	//Spawn the pickup
	if (m_SecondsSinceDespawn > m_SecondsToWait && !m_Spawned)
	{
		m_Spawned = true;
		m_SecondsSinceSpawn = 0;
		spawn();
	}
}

void Pickup::upgrade()
{
	//50%  upgrade
	if (m_Type == 1)
	{
		m_Value += (HEALTH_START_VALUE * .5);
	}
	else
	{
		m_Value += (AMMO_START_VALUE * .5);
	}

	//Make them more frequent and last longer
	m_SecondsToLive += (START_SECONDS_TO_LIVE / 10);
	m_SecondsToWait -= (START_WAIT_TIME / 10);
}