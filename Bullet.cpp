#include "Bullet.h"

//The constructor only sets the size of the bullet
Bullet::Bullet()
{
	m_BulletShape.setSize(sf::Vector2f(2, 2));
}

void Bullet::shoot(float startX, float startY, float targetX, float targetY)
{
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	//Calculate the gradient (the slope) of the bullet flight path
	float gradient = (startX - targetX) / (startY- targetY);

	//Any gradient less than 1 needs to be negative
	//The screen coordinates start from origin(0, 0), top left corner => we are only positive
	if (gradient < 0)
	{
		gradient *= -1;
	}	

	//Calculate the ratio between x and y, it braks down the speed into component on x and y axes
	//based on the travelling direction (gradient)
	//1 + gradient role: *if the gradient is 1, the bullet is moving equally on x and y axes (45 degrees), gradient>1 it moves more verically
	//the ratioXY is less after the division and therefore moves more on the y and it's speed on x axys is less
	//gradient < 1 it means the bullet moves more horizontally => ratioXY will be larger and that means more movement on x axis*
	//basically how much will the bullet move on y axis (vertically)
	float ratioXY = m_BulletSpeed / (1 + gradient);

	//Set the speed horizontally and verically
	//ratioXY * gradient = how much speed on the X axis based on the slope
	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;

	//Point the bullet in the right direction
	if (targetX < startX)
	{
		m_BulletDistanceX *= -1;
	}

	if (targetY < startY)
	{
		m_BulletDistanceY *= -1;
	}

	//Set a max range of 1000 pixels in any direction
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	//Set it's position, ready to be drawn
	m_BulletShape.setPosition(m_Position);
}
void Bullet::stop()
{
	m_InFlight = false;
}

bool Bullet::isInFlight()
{
	return m_InFlight;
}

FloatRect Bullet::getPosition()
{
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}

void Bullet::update(float elapsedTime)
{
	//Update the bullet position variables
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	//Move the bullet
	m_BulletShape.setPosition(m_Position);

	//Check if bullet s out of range
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX || m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}
}