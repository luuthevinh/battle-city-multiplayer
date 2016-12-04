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

	_remainMoveForTurn = 0.0f;

	return true;
}

void Tank::update(float dt)
{
	this->turnWithInputQueue(dt);

	this->updateWithInputQueue(dt);

	this->updatePosition(dt);

	this->updateBoundingBoxPosition();

	// reset hướng va chạm
	for (auto it = _objectCollidingCounter.begin(); it != _objectCollidingCounter.end(); it++)
	{
		it->second = 0;
	}

	_collidingSide = eDirection::NONE;
}

void Tank::updatePosition(float dt)
{
	if (!this->hasStatus(eStatus::RUNNING))
	{
		if (_velocity != 0)
		{
			_velocity = 0;

			_inputTurns.clear();
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
	if (_inputTurns.size() <= 0)
		return;

	eDirection direction = _inputTurns.back();

	if (direction == eDirection::NONE || _direction == direction)
	{
		return;
	}

	// nếu mà ko có va chạm hướng đó hoặc va chạm hơn 2 cái thì update direction luôn
	if (!this->isCollidingAtSide(direction) || _objectCollidingCounter.at(direction) > 1)
	{
		this->updateDirection(direction);
		return;
	}

	auto position = this->getPosition();
	
	int unit = 16;
	float integral = 0.0f;
	float fractional = 0.0f;	// phần lẻ sau dấu phẩy của vị trí ban đầu
	fractional = modf(position.x, &integral);				
	float remainX = unit - (((int)integral % unit) + fractional);

	float integralY = 0.0f;
	float fractionalY = 0.0f;
	fractionalY = modf(position.y, &integralY);
	float remainY = unit - (((int)integralY % unit) + fractionalY);

	_remainMoveForTurn = fractional > fractionalY ? remainX : remainY;

	//if (_remainMoveForTurn > unit / 2)
	//{
	//	_remainMoveForTurn = 0.0f;
	//	this->updateDirection(direction);
	//	return;
	//}
	
	_inputTurns.clear();
	_inputTurns.push_back(direction);
	_inputTurns.push_back(_direction);

	printf("begin remain: %.2f, current dir: %d, dir: %d\n", _remainMoveForTurn, _direction, direction);

	return;
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

	if (_remainMoveForTurn > distance)
	{
		_remainMoveForTurn -= distance;
		printf("remain: %.2f\n", _remainMoveForTurn);
	}
	else if (_remainMoveForTurn > 0.0f)
	{
		distance = _remainMoveForTurn;
		_remainMoveForTurn = 0.0f;
		printf("end: %.2f\n", _remainMoveForTurn);
	}

	

	switch (_direction)
	{
	case LEFT:
		this->setPosition(this->getPosition().x - distance, this->getPosition().y);
		//_collidingSide = (eDirection)(_collidingSide & ~RIGHT);
		break;
	case UP:
		this->setPosition(this->getPosition().x, this->getPosition().y + distance);
		//_collidingSide = (eDirection)(_collidingSide & ~DOWN);
		break;
	case RIGHT:
		this->setPosition(this->getPosition().x + distance, this->getPosition().y);
		//_collidingSide = (eDirection)(_collidingSide & ~LEFT);
		break;
	case DOWN:
		this->setPosition(this->getPosition().x, this->getPosition().y - distance);
		//_collidingSide = (eDirection)(_collidingSide & ~UP);
		break;
	default:
		break;
	}
}

void Tank::checkCollidingSide(GameObject& other)
{
	auto otherRect = other.getBoundingBox();

	auto side = _collisionChecker->getSideOverlap(this->getBoundingBox(), otherRect);

	if (side != eDirection::NONE)
	{
		_objectCollidingCounter[side]++;
	}

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

	if(_remainMoveForTurn == 0)
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

	if (other.getId() == eObjectId::BULLET)
	{
		return;
	}

	this->updateBoundingBoxPosition();

	this->checkCollidingSide(other);

	eDirection result;
	float time = _collisionChecker->checkCollision(*this, other, result, dt);

	// có va chạm
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
	

	float moveX, moveY;

	if (_collisionChecker->isColliding(this->getBoundingBox(), other.getBoundingBox(), moveX, moveY, dt))
	{
		this->setPosition(this->getPosition().x + moveX, this->getPosition().y + moveY);
	}
}

void Tank::setDirection(eDirection direction)
{
	if (_inputTurns.empty())
	{
		_inputTurns.push_back(direction);
	}
}
