#include "Player.h"

Player::Player()
{
	_velocity = 0.0f;
	_direction = eDirection::UP;
	_x = 100;
	_y = 100;
}

Player::~Player()
{
}

void Player::update(float dt)
{
	updatePosition(dt);
}

void Player::updatePosition(float dt)
{
	if (_velocity == 0.0f)
		return;

	switch (_direction)
	{
	case LEFT:
		this->setPosition(_x - _velocity * dt, _y);
		break;
	case UP:
		this->setPosition(_x, _y + _velocity * dt);
		break;
	case RIGHT:
		this->setPosition(_x + _velocity * dt, _y);
		break;
	case DOWN:
		this->setPosition(_x, _y - _velocity * dt);
		break;
	default:
		break;
	}
}

void Player::setPosition(float x, float y)
{
	_x = x;
	_y = y;
}

void Player::setDirection(eDirection dir)
{
	_direction = dir;
}

eDirection Player::getDirection()
{
	return _direction;
}

void Player::setVelocity(float velocity)
{
	_velocity = velocity;
}

float Player::getPositionX()
{
	return _x;
}

float Player::getPositionY()
{
	return _y;
}

float Player::getVelocity()
{
	return _velocity;
}
