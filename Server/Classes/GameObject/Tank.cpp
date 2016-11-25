#include "Tank.h"
#include "..\Base\AABB.h"
#include <cmath>
#include <stdio.h>

Tank::Tank(eObjectId id) : GameObject(id),
	_velocity(0)
{
	this->init();
}

Tank::Tank(Buffer& data) : GameObject(data),
	_velocity(0)
{
	this->init();
}

Tank::~Tank()
{
	delete _collisionChecker;
}

bool Tank::init()
{
	_boudingBox.width = 32;
	_boudingBox.height = 32;

	this->updateBoundingBoxPosition();

	_collisionChecker = new AABB();
	_collisionChecker->setOffset(2);

	_inputTurns.push_back(_direction);

	return true;
}

void Tank::update(float dt)
{
	if (_inputTurns.size() > 0)
	{
		this->turnWithInputQueue(dt);
	}

	this->updateWithInputQueue(dt);
	
	this->updatePosition(dt);

	this->updateBoundingBoxPosition();

	// reset cạnh va chạm
	_collidingSide = eDirection::NONE;
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

	this->move(_velocity * dt);
}

void Tank::updatePosition(float dt, float distance)
{
	if (distance != 0)
	{
		this->onChanged();
	}

	this->move(distance);
}

void Tank::updateBoundingBoxPosition()
{
	// update bounding box
	_boudingBox.position.x = this->getPosition().x - _boudingBox.width / 2;
	_boudingBox.position.y = this->getPosition().y - _boudingBox.height / 2;
}

void Tank::turnWithInputQueue(float dt)
{
	eDirection direction = _inputTurns.back();

	if (direction == eDirection::NONE || _direction == direction)
		return;

	if (!this->isCollidingAtSide(direction))
	{
		this->updateDirection(direction);
		return;
	}

	auto position = this->getPosition();
	
	int unit = 16;
	float integral = 0.0f;
	float fractional = 0.0f;
	fractional = modf(position.x, &integral);
	float remainX = unit - (((int)integral % unit) + fractional);

	float integralY = 0.0f;
	float fractionalY = 0.0f;
	fractionalY = modf(position.y, &integralY);
	float remainY = unit - (((int)integralY % unit) + fractionalY);

	int number = 0;
	switch (direction)
	{
	case NONE:
		break;
	case LEFT:
	case RIGHT:
		number += remainY / (TANK_NORMAL_VELOCITY * dt);
		break;
	case UP:
	case DOWN:
		number += remainX / (TANK_NORMAL_VELOCITY * dt);
		break;
	default:
		break;
	}

	_inputTurns.push_back(direction);

	// TODO: thêm di chuyển quãng đường còn lại nhỏ hơn vận tốc
	this->generateInput(_direction, number);
}

void Tank::updateWithInputQueue(float dt)
{
	auto direction = this->getDirectionInQueue();
	this->updateDirection(direction);
}

void Tank::generateInput(eDirection direction, int number)
{
	for (int i = 0; i < number; i++)
	{
		_inputTurns.push_back(direction);
	}
}

void Tank::move(float distance)
{
	if (this->isCollidingAtSide(_direction) || distance == 0)
		return;

	switch (_direction)
	{
	case LEFT:
		this->setPosition(this->getPosition().x - distance, this->getPosition().y);
		_collidingSide = (eDirection)(_collidingSide & ~RIGHT);
		break;
	case UP:
		this->setPosition(this->getPosition().x, this->getPosition().y + distance);
		_collidingSide = (eDirection)(_collidingSide & ~DOWN);
		break;
	case RIGHT:
		this->setPosition(this->getPosition().x + distance, this->getPosition().y);
		_collidingSide = (eDirection)(_collidingSide & ~LEFT);
		break;
	case DOWN:
		this->setPosition(this->getPosition().x, this->getPosition().y - distance);
		_collidingSide = (eDirection)(_collidingSide & ~UP);
		break;
	default:
		break;
	}
}

void Tank::checkCollidingSide(const Rect & otherRect)
{
	auto side = _collisionChecker->getSideOverlap(this->getBoundingBox(), otherRect);

	_collidingSide = (eDirection)(_collidingSide | side);
}

bool Tank::isCollidingAtSide(eDirection side)
{
	return (_collidingSide & side) == side;
}

eDirection Tank::getDirectionInQueue()
{
	if (_inputTurns.size() == 0)
		return _direction;

	auto ret = _inputTurns.back();
	_inputTurns.pop_back();

	return ret;
}

void Tank::updateDirection(eDirection direction)
{
	if (_direction == direction)
		return;

	_direction = direction;
}

void Tank::setVelocity(float velocity)
{
	_velocity = velocity;
}

float Tank::getVeloctiy()
{
	return _velocity;
}

Vector2 Tank::getVelocity() const
{
	switch (_direction)
	{
	case LEFT:
		return Vector2(-_velocity, 0.0f);
	case UP:
		return Vector2(0.0f, _velocity);
	case RIGHT:
		return Vector2(_velocity, 0.0f);
	case DOWN:
		return Vector2(0.0f, -_velocity);
	default:
		break;
	}
	
	return Vector2(0.0f, 0.0f);
}

void Tank::onChanged()
{
	this->setChanged(true);
}

void Tank::checkCollision(GameObject & other, float dt)
{
	if (!this->canCollisionWith(other.getCategoryBitmask()))
		return;

	this->updateBoundingBoxPosition();

	this->checkCollidingSide(other.getBoundingBox());

	eDirection result;

	float time = _collisionChecker->checkCollision(*this, other, result, dt);

	// có va chạm
	if (result != eDirection::NONE)
	{
		if (time < 1.0f)
		{
			float distance = (_velocity * dt) * time;
			this->updatePosition(dt, distance);

			// ko cho chạy nữa
			if (_inputTurns.size() == 0)
			{
				this->removeStatus(eStatus::RUNNING);
			}

			this->onChanged();
		}
	}

	float moveX, moveY;

	if (_collisionChecker->isColliding(this->getBoundingBox(), other.getBoundingBox(), moveX, moveY, dt))
	{
		this->setPosition(this->getPosition().x + moveX, this->getPosition().y + moveY);
	}
}

void Tank::setDirection(eDirection direction)
{
	if (_inputTurns.empty() || _inputTurns.back() != direction)
	{
		_inputTurns.push_back(direction);
	}
}