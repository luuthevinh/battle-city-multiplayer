﻿#include "Tank.h"

Tank::Tank(eObjectId id) : GameObject(id),
	_velocity(0)
{
}

Tank::Tank(Buffer& data) : GameObject(data),
	_velocity(0)
{
}

Tank::~Tank()
{
}

bool Tank::init()
{
	
	return true;
}

void Tank::update(float dt)
{
	// update postion
	this->updatePosition(dt);
}

void Tank::updatePosition(float dt)
{
	if (!this->hasStatus(eStatus::RUNNING))
	{
		if (_velocity != 0)
		{
			_velocity = 0;
		}
	}
	else
	{
		if (_velocity == 0)
		{
			_velocity = TANK_NORMAL_VELOCITY;
		}
	}

	if (_velocity != 0)
	{
		this->onChanged();
	}

	switch (_direction)
	{
	case LEFT:
		this->setPosition(this->getPosition().x - _velocity * dt, this->getPosition().y);
		break;
	case UP:
		this->setPosition(this->getPosition().x, this->getPosition().y + _velocity * dt);
		break;
	case RIGHT:
		this->setPosition(this->getPosition().x + _velocity * dt, this->getPosition().y);
		break;
	case DOWN:
		this->setPosition(this->getPosition().x, this->getPosition().y - _velocity * dt);
		break;
	default:
		break;
	}
}

void Tank::setVelocity(float velocity)
{
	_velocity = velocity;
}

float Tank::getVeloctiy()
{
	return _velocity;
}

void Tank::onChanged()
{
	this->setChanged(true);
}
